#ifndef _PARAMS_H_
#define _PARAMS_H_

#include <cstdint>

extern const char *input_iface;
extern const char *output_iface;

class __attribute__((packed)) FiveTuple
{
public:
    uint32_t srcIp;
    uint16_t srcPort;
    uint32_t dstIp;
    uint16_t dstPort;
    uint8_t protocol;
};

extern const uint32_t SKETCH_MEM; // in byte
extern const double WINDOW_SIZE; // in second

#endif // _PARAMS_H_
