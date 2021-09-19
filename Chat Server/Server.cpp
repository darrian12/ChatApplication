//#include "util.h"
#include "TcpListener.h"

#include <sstream>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void ListenerMessageReceived(Network::TcpListener* listener, int client, int sender, const std::string& msg);

int main()
{
    Network::TcpListener server("127.0.0.1", 54000, ListenerMessageReceived);
    if (server.Init())
    {
        server.Run();
    }

    return 0;
}

void ListenerMessageReceived(Network::TcpListener* listener, int client, int sender, const std::string& msg)
{
    std::ostringstream ss;
    ss << "SOCKET #" << sender << "> " << msg;

    listener->SendMsg(client, ss.str());
}