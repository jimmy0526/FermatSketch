#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include <pcap.h>
#include <libnet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <iostream>
// #include <../FermatSketch/fermat.h>

using namespace std;

class PacketHandler
{
public:
    bool init(const char *in_iface, const char* out_iface);
    void run();
    void stop();

private:
    pcap_t *handle;
    libnet_t *l;

    void _record(const sockaddr_in &srcIp, uint16_t srcPort, const sockaddr_in &dstIp, uint16_t dstPort, uint8_t protocol);
    void _handler(const struct pcap_pkthdr *pkthdr, const u_char *packet);
};

#endif // _PACKET_HANDLER_H_
