#include <iostream>
#include <csignal>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>


#include "packet_handler.h"
#include "shared_resource.h"

using namespace std;

class LocalAgent
{
public:
    void record()
    {
        int cnt = 0;
        while (running)
        {
            // should be replaced by packet_handler
            {
                resource.addToCurrentSketch(++cnt);

                this_thread::sleep_for(chrono::seconds(1));
            }
        }
    }

    void report()
    {
        while (running) {
            this_thread::sleep_for(chrono::duration<double>(WINDOW_SIZE));

            // switch to next time window
            resource.switchWindow();

            this_thread::sleep_for(chrono::duration<double>(1));

            // should be replaced by report sketch to analyzer
            {
                auto sketch = resource.getInactiveSketch();
                resource.resetInactiveSketch();

                // unordered_map<uint32_t, int> result;
                // sketch.Decode(result);

                // for (auto &kv : result)
                // {
                //     cout << kv.first << " " << kv.second << endl;
                // }
                cout << "ok" << endl;
            }
        }
    }

    void start()
    {
        running = true;
        record_thread = thread(&LocalAgent::record, this);
        report_thread = thread(&LocalAgent::report, this);
    }

    void stop()
    {
        running = false;
        if (record_thread.joinable()) record_thread.join();
        if (report_thread.joinable()) report_thread.join();
    }

    static void signalHandler(int signal)
    {
        agent.stop();
    }

    atomic<bool> running = false;
    static LocalAgent agent;

private:
    thread record_thread, report_thread;
    SharedResource resource;
};

LocalAgent LocalAgent::agent;

int main()
{
    signal(SIGINT, LocalAgent::signalHandler);

    LocalAgent::agent.start();

    while (LocalAgent::agent.running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

// int main()
// {
//     PacketHandler ph;
//     ph.init("ens1f1", "ens1f1");
//     ph.run();
//     ph.stop();
//     return 0;
// }