#include "packet_handler.h"

bool PacketHandler::init(const char *in_iface, const char* out_iface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    l = libnet_init(LIBNET_RAW4, out_iface, errbuf);
    if (l == NULL)
    {
        fprintf(stderr, "libnet_init() fail: %s\n", errbuf);
        return false;
    }

    handle = pcap_open_live(in_iface, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        fprintf(stderr, "pcap_open_live() fail: %s\n", errbuf);
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

void PacketHandler::_record(const sockaddr_in &srcIp, uint16_t srcPort, const sockaddr_in &dstIp, uint16_t dstPort, uint8_t protocol)
{
    // TODO: insert to sketch
    // call Fermat
    cout << "<" << inet_ntoa(srcIp.sin_addr) << "," << ntohs(srcPort) 
         << "," << inet_ntoa(dstIp.sin_addr) << "," << ntohs(dstPort)
         << "," << (int)protocol << endl;
}

void PacketHandler::_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    const struct ip *ip_header = (struct ip *)(packet + 14);
    uint16_t ip_len = ntohs(ip_header->ip_len);  // 整个 IP 包的长度
    uint16_t ip_header_len = ip_header->ip_hl * 4;  // IP 头部长度
    const uint8_t *payload = (uint8_t *)ip_header + ip_header_len;  // IP 负载
    uint16_t payload_len = ip_len - ip_header_len;  // IP 负载长度

    if (ip_header->ip_p == IPPROTO_TCP)
    {
        const struct tcphdr *tcp_header = (struct tcphdr *)payload;

        // 提取 TCP 五元组信息
        // std::cout << "TCP Packet: " 
        //           << inet_ntoa(ip_header->ip_src) << ":" << ntohs(tcp_header->th_sport) << " -> "
        //           << inet_ntoa(ip_header->ip_dst) << ":" << ntohs(tcp_header->th_dport) << std::endl;

        return;

    }
    else if (ip_header->ip_p == IPPROTO_UDP)
    {
        const struct udphdr *udp_header = (struct udphdr *)payload;

        // 提取 UDP 五元组信息
        // _record
        std::cout << "UDP Packet: "
                  << inet_ntoa(ip_header->ip_src) << ":" << ntohs(udp_header->uh_sport) << " -> "
                  << inet_ntoa(ip_header->ip_dst) << ":" << ntohs(udp_header->uh_dport) << std::endl;
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
        payload,
        payload_len,
        l,
        0
    );

    if (ptag == -1)
    {
        fprintf(stderr, "cannot construct packet header: %s\n", libnet_geterror(l));
        return;
    }

    if (libnet_write(l) == -1)
    {
        fprintf(stderr, "send error: %s\n", libnet_geterror(l));
        return;
    }

    libnet_clear_packet(l);
}