#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include <pcap.h>
#include <libnet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "shared_resource.h"

using namespace std;

class PacketHandler
{
public:
    bool init(const char *in_iface, const char *out_iface, SharedResource *_resource);
    void run();
    void stop();

private:
    SharedResource *resource;

    pcap_t *handle;
    libnet_t *l;

    void _record(uint32_t srcIp, uint16_t srcPort, uint32_t dstIp, uint16_t dstPort, uint8_t protocol);
    void _handler(const struct pcap_pkthdr *pkthdr, const u_char *packet);
};

#endif // _PACKET_HANDLER_H_
