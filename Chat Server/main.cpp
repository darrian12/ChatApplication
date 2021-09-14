#include "util.h"

#include <sstream>
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
        WSACleanup();
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
    LOG("Server is listening for connections..");

    fd_set master;
    FD_ZERO(&master);

    FD_SET(listenSocket, &master);

    while (true)
    {
        fd_set copyMaster = master;

        int socketCount = select(0, &copyMaster, nullptr, nullptr, nullptr);

        for (int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copyMaster.fd_array[i];
            if (sock == listenSocket)
            {
                sockaddr_in client;
                int clientSize = sizeof(client);

                SOCKET clientSocket = accept(listenSocket, (sockaddr*)&client, &clientSize);
                if (clientSocket == INVALID_SOCKET)
                {
                    LOG_ERROR("Failed to create client socket with error - " + std::to_string(WSAGetLastError()));
                    continue;
                }

                FD_SET(clientSocket, &master);

                std::string msg = "Succesfully connected to the server!\n";
                send(clientSocket, msg.c_str(), msg.size() + 1, 0);
            }
            else
            {
                char buf[4096];
                ZeroMemory(buf, 4096);

                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived <= 0)
                {
                    closesocket(sock);
                    FD_CLR(sock, &master);
                    LOG("Client disconnected");
                }
                else
                {
                    std::cout << "[CLIENT #" << sock << "]: " << std::string(buf, 0, bytesReceived) << "\n";

                    for (unsigned int i = 0; i < master.fd_count; i++)
                    {
                        SOCKET outSock = master.fd_array[i];
                        if (outSock != listenSocket && outSock != sock)
                        {
                            std::ostringstream ss;
                            ss << "SOCKET #" << sock << "> " << buf;

                            //std::cout << "sending to #" << outSock << "\n";
                            send(outSock, ss.str().c_str(), ss.str().size() + 1, 0);
                        }
                    }
                }
            }
        }
    }

    LOG("Shutting down server..");
    closesocket(listenSocket);
    WSACleanup();

    system("pause");

    return 0;
}