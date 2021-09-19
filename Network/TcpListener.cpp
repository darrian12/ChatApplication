#include "TcpListener.h"

#include <iostream>

Network::TcpListener::TcpListener(const std::string& ipAddress, int port, MessageReceivedHandler handler)
    : m_ipAddress(ipAddress), m_port(port), m_messageReceivedHandler(handler)
{
}

Network::TcpListener::~TcpListener()
{
    Cleanup();
}

bool Network::TcpListener::Init()
{
    return InitWinsock(2, 2);
}

void Network::TcpListener::Run()
{
    SOCKET listenSocket = CreateListenSocket();
    if (listenSocket == SOCKET_ERROR)
        return;

    fd_set master;
    FD_ZERO(&master);

    FD_SET(listenSocket, &master);

    while (true)
    {
        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copy.fd_array[i];
            if (sock == listenSocket)
            {
                sockaddr_in client;
                int clientSize = sizeof(client);

                SOCKET clientSocket = WaitForConnection(listenSocket, (sockaddr*)&client, &clientSize);
                if (clientSocket == SOCKET_ERROR)
                    continue;

                FD_SET(clientSocket, &master);

                std::string msg = "Succesfully connected to the server!\n";
                SendMsg(clientSocket, msg);
            }
            else
            {
                char buf[MAX_BUFFER_SIZE];
                ZeroMemory(buf, MAX_BUFFER_SIZE);

                int bytesReceived = recv(sock, buf, MAX_BUFFER_SIZE, 0);
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
                            if (m_messageReceivedHandler != nullptr)
                            {
                                m_messageReceivedHandler(this, outSock, sock, std::string(buf, 0, bytesReceived));
                            }
                        }
                    }
                }
            }
        }
    }

    closesocket(listenSocket);
}

SOCKET Network::TcpListener::CreateListenSocket()
{
    SOCKET listenSocket = CreateSocket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == SOCKET_ERROR)
    {
        return SOCKET_ERROR;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

    int bindOk = bind(listenSocket, (const sockaddr*)&hint, sizeof(hint));
    if (bindOk == SOCKET_ERROR)
    {
        LOG_ERROR("Failed to bind socket with error #" + std::to_string(WSAGetLastError()));
        return SOCKET_ERROR;
    }

    int listenOk = listen(listenSocket, SOMAXCONN);
    if (listenOk == SOCKET_ERROR)
    {
        LOG_ERROR("Failed to listen socket with error #" + std::to_string(WSAGetLastError()));
        return SOCKET_ERROR;
    }

    return listenSocket;
}

SOCKET Network::TcpListener::WaitForConnection(SOCKET listenSocket, sockaddr* addr, int* addrlen)
{
    SOCKET client = accept(listenSocket, addr, addrlen);
    if (client == INVALID_SOCKET)
    {
        LOG_ERROR("Failed to accept socket with error #" + std::to_string(WSAGetLastError()));
        return SOCKET_ERROR;
    }

    return client;
}
