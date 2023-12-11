#ifndef _PARAMS_H_
#define _PARAMS_H_

class __attribute__((packed)) FiveTuple
{
public:
    uint32_t srcIp;
    uint16_t srcPort;
    uint32_t dstIp;
    uint16_t dstPort;
    uint8_t protocol;
};

const uint32_t SKETCH_MEM = 1 << 20; // in byte
const double WINDOW_SIZE = 10; // in second

#endif // _PARAMS_H_
