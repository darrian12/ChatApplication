#include "util.h"

#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsaData);
    if (wsOk != 0)
    {
        LOG_ERROR("WSA Startup failed with error - " + std::to_string(wsOk));
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); // 0 - any protocol | IPPROTO_TCP
    if (listenSocket == INVALID_SOCKET)
    {
        LOG_ERROR("Failed to create socket with error - " + std::to_string(WSAGetLastError()));
        return 1;
    }

    int port = 54000;
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    // htons = host to network short
    hint.sin_port = htons(port);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listenSocket, (const sockaddr*)&hint, sizeof(hint));
    
    listen(listenSocket, SOMAXCONN);

    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET)
    {
        LOG_ERROR("Failed to create client socket with error - " + std::to_string(WSAGetLastError()));
        return 1;
    }

    char host[NI_MAXHOST];  // client remote name
    char service[NI_MAXSERV]; // service of client (port)

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0))
    {
        LOG(std::string(host) + " connected on port " + std::string(service));
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        LOG(std::string(host) + " connected on port " + std::to_string(ntohs(client.sin_port)));
    }

    closesocket(listenSocket);

    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            LOG_ERROR("failed recv() with error - " + std::to_string(WSAGetLastError()));
            break;
        }

        if (bytesReceived == 0)
        {
            LOG("client disconnected");
            break;
        }

        if (buf[1] == '\n')
        {
            continue;
        }

        std::cout << "[CLIENT]: " << buf << " | bytes: " << bytesReceived << "\n";
        //send(clientSocket, buf, bytesReceived + 1, 0);
    }

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}