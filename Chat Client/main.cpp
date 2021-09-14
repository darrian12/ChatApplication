#include "util.h"

#include <iostream>
#include <thread>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void Listening(SOCKET s)
{
    char buf[4096];
    std::string line;

    while (s != INVALID_SOCKET)
    {
        ZeroMemory(buf, 4096);

        int bytesIn = recv(s, buf, 4096, 0);
        if (bytesIn > 0)
        {
            std::cout << buf << "\n";
        }
    }
}

int main()
{
    std::string ip = "127.0.0.1"; // address of server
    int port = 54000; // port of server

    WSAData data;
    WORD ver = MAKEWORD(2, 2);

    int wsaResult = WSAStartup(ver, &data);
    if (wsaResult != 0)
    {
        LOG_ERROR("Can't start Winsock with error - " + std::to_string(wsaResult));
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        LOG_ERROR("Invalid socket with error - " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (const sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        LOG_ERROR("Can't connect to server with error - " + std::to_string(WSAGetLastError()));
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //LOG("Succesfully connected to server!");

    std::string input;
    std::thread listenWorker(Listening, sock);

    do
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.size() > 0)
        {
            send(sock, input.c_str(), input.size() + 1, 0);
        }
    } while (input.size() > 0);

    closesocket(sock);
    WSACleanup();

    listenWorker.join();

    return 0;
}
