#include "local_agent.h"

LocalAgent LocalAgent::agent;

bool LocalAgent::init()
{
    // init shared resource
    if (!handler.init(input_iface, output_iface, &resource))
        return false;
    if (!uploader.init(&resource))
        return false;
    return true;
}

void LocalAgent::start()
{
    running = true;
    thread_ph = thread(&PacketHandler::run, &handler);
    thread_su = thread(&SketchUploader::run, &uploader);
}

void LocalAgent::stop()
{
    running = false;
    if (thread_ph.joinable()) thread_ph.join();
    if (thread_su.joinable()) thread_su.join();
}

void LocalAgent::signalHandler(int signal)
{
    agent.stop();
}
