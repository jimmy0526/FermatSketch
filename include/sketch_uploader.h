#ifndef _SKETCH_UPLOADER_H_
#define _SKETCH_UPLOADER_H_

#include <chrono>
#include <thread>

#include "shared_resource.h"

class SketchUploader
{
public:
    bool init(SharedResource *_resource);
    void run();
    void stop();

private:
    SharedResource *resource;
};

#endif // _SKETCH_UPLOADER_H_