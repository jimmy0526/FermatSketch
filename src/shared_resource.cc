#include "shared_resource.h"

void SharedResource::addToCurrentSketch(uint32_t srcIp /* const FiveTuple &tuple */)
{
    // cout << window_id << endl;
    sketch[window_id].Insert_one(srcIp);
}

void SharedResource::switchWindow()
{
    window_id ^= 1;
}

Fermat SharedResource::getInactiveSketch()
{
    // cout << "read sketch " << (window_id ^ 1) << endl;
    return sketch[window_id ^ 1];
}

void SharedResource::resetInactiveSketch()
{
    /*
     * Although it is confusing, this approach is correct: getInactiveSketch()
     * constructs a new sketch, however, it only copies the pointers, which
     * point to the original memory addresses. When this sketch is destructed,
     * it releases this memory, thus not causing a memory leak.
     * A better approach would be not to release and reallocate memory, but that
     * would require modifying the underlying implementation of FermatSketch.
     */
    sketch[window_id ^ 1].create_array();
}
