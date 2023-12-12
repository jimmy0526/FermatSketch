#ifndef _LOCAL_AGENT_H_
#define _LOCAL_AGENT_H_

#include <csignal>
#include <thread>

#include "shared_resource.h"
#include "packet_handler.h"
#include "sketch_uploader.h"

class LocalAgent
{
public:
    atomic<bool> running = false;
    static LocalAgent agent;

    bool init();
    void start();
    void stop();
    static void signalHandler(int signal);

private:
    PacketHandler handler;
    SketchUploader uploader;
    thread thread_ph, thread_su;
    SharedResource resource;
};

#endif // _LOCAL_AGENT_H_