#ifndef _UTILS_H_
#define _UTILS_H_

class __attribute__((packed)) FiveTuple
{
public:
    uint32_t srcIp;
    uint16_t srcPort;
    uint32_t dstIp;
    uint16_t dstPort;
    uint8_t protocol;
};

#endif // _UTILS_H_
