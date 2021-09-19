#include "util.h"
#include "TcpConnecter.h"

#include <iostream>
#include <thread>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void Listening(int socket)
{
    char buf[4096];
    std::string line;

    while (socket != INVALID_SOCKET)
    {
        ZeroMemory(buf, 4096);

        int bytesIn = recv(socket, buf, 4096, 0);
        if (bytesIn > 0)
        {
            std::cout << buf << "\n";
        }
    }
}

int main()
{
    Network::TcpConnecter client("127.0.0.1", 54000, Listening);
    if (client.Init())
    {
        client.Run();
        client.GetListenWorker().join();
    }

    return 0;
}
