#include "sketch_uploader.h"

bool SketchUploader::init(SharedResource *_resource)
{
    resource = _resource;

    // tcp connect

    return true;
}

void SketchUploader::run()
{
    while (true)
    {
        this_thread::sleep_for(chrono::duration<double>(WINDOW_SIZE));

        resource->switchWindow();

        this_thread::sleep_for(chrono::duration<double>(WINDOW_SIZE / 10));

        auto sketch = resource->getInactiveSketch();

        resource->resetInactiveSketch();

        unordered_map<uint32_t, int> result;
        sketch.Decode(result);

        // should be replaced by upload sketch to analyzer via tcp
        {
            for (auto &kv : result)
            {
                cout << hex << kv.first << " " << dec << kv.second << endl;
            }
            cout << "ok" << endl;
        }
    }
}

void SketchUploader::stop()
{
    // close tcp connection
}