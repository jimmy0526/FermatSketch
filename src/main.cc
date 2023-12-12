#include <iostream>

#include "local_agent.h"

using namespace std;

int main()
{
    signal(SIGINT, LocalAgent::signalHandler);

    LocalAgent::agent.init();
    
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