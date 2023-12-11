#ifndef _SHARED_RESOURCE_H_
#define _SHARED_RESOURCE_H_

#include <atomic>

#include "FermatSketch/fermat.h"
#include "params.h"

using namespace std;

class SharedResource
{
public:
    void addToCurrentSketch(uint32_t srcIp /* const FiveTuple &tuple */);
    void switchWindow();
    Fermat getInactiveSketch();
    void resetInactiveSketch();

private:
    Fermat sketch[2] = {Fermat(SKETCH_MEM, false), Fermat(SKETCH_MEM, false)};
    atomic<int> window_id = 0;
};

#endif // _SHARED_RESOURCE_H_
