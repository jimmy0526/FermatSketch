#include "packet_handler.h"

bool PacketHandler::init(const char *in_iface, const char* out_iface, SharedResource *_resource)
{
    resource = _resource;

    char errbuf[PCAP_ERRBUF_SIZE];

    l = libnet_init(LIBNET_RAW4, out_iface, errbuf);
    if (l == NULL)
    {
        cerr << "libnet_init() fail: " << errbuf << endl;
        return false;
    }

    handle = pcap_open_live(in_iface, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        cerr << "pcap_open_live() fail: " << errbuf << endl;
        libnet_destroy(l);
        return false;
    }

    struct bpf_program filter;
    char filter_exp[] = "tcp or udp";
    if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
    {

        std::cerr << "pcap_compile() fail: " << pcap_geterr(handle) << std::endl;
        return false;
    }

    if (pcap_setfilter(handle, &filter) == -1)
    {
        std::cerr << "pcap_setfilter() fail: " << pcap_geterr(handle) << std::endl;
        return false;
    }

    return true;
}

void PacketHandler::run()
{
    auto callback = [](u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
        reinterpret_cast<PacketHandler *>(user)->_handler(pkthdr, packet);
    };

    pcap_loop(handle, -1, callback, reinterpret_cast<u_char*>(this));
}

void PacketHandler::stop()
{
    pcap_close(handle);
    libnet_destroy(l);
}

void PacketHandler::_record(uint32_t srcIp, uint16_t srcPort, uint32_t dstIp, uint16_t dstPort, uint8_t protocol)
{
    resource->addToCurrentSketch(srcIp /* 5-tuple */);
}

void PacketHandler::_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    const struct ip *ip_header = (struct ip *)(packet + 14);
    uint16_t ip_len = ntohs(ip_header->ip_len);
    uint16_t ip_header_len = ip_header->ip_hl * 4;
    const uint8_t *ip_payload = (uint8_t *)ip_header + ip_header_len;
    uint16_t ip_payload_len = ip_len - ip_header_len;

    if (ip_header->ip_p == IPPROTO_TCP)
    {
        const struct tcphdr *tcp_header = (struct tcphdr *)ip_payload;
        _record(ntohl(ip_header->ip_src.s_addr), ntohs(tcp_header->th_sport),
                ntohl(ip_header->ip_dst.s_addr), ntohs(tcp_header->th_dport),
                IPPROTO_TCP);
    }
    else if (ip_header->ip_p == IPPROTO_UDP)
    {
        const struct udphdr *udp_header = (struct udphdr *)ip_payload;
        _record(ntohl(ip_header->ip_src.s_addr), ntohs(udp_header->uh_sport),
                ntohl(ip_header->ip_dst.s_addr), ntohs(udp_header->uh_dport),
                IPPROTO_UDP);

        // std::cout << "UDP Packet: "
        //           << inet_ntoa(ip_header->ip_src) << ":" << ntohs(udp_header->uh_sport) << " -> "
        //           << inet_ntoa(ip_header->ip_dst) << ":" << ntohs(udp_header->uh_dport) << std::endl;
    }

    // TODO: need further test, need config iface
    libnet_ptag_t ptag = libnet_build_ipv4(
        ip_len,
        ip_header->ip_tos,
        ntohs(ip_header->ip_id),
        ntohs(ip_header->ip_off),
        ip_header->ip_ttl,
        ip_header->ip_p,
        0,
        ip_header->ip_dst.s_addr,
        ip_header->ip_src.s_addr, 
        ip_payload,
        ip_payload_len,
        l,
        0
    );

    if (ptag == -1)
    {
        cerr << "cannot construct packet header: " << libnet_geterror(l) << endl;
        return;
    }

    if (libnet_write(l) == -1)
    {
        cerr << "send error: " << libnet_geterror(l) << endl;
        return;
    }

    libnet_clear_packet(l);
}