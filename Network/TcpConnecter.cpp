#include "TcpConnecter.h"

#include <iostream>

Network::TcpConnecter::TcpConnecter(const std::string& ipAddress, int port, ListeningHandler handler)
    : m_ipAddress(ipAddress), m_port(port), m_listeningHandler(handler)
{
}

Network::TcpConnecter::~TcpConnecter()
{
    Cleanup();
}

bool Network::TcpConnecter::Init()
{
    return InitWinsock(2, 2);
}

void Network::TcpConnecter::Run()
{
    SOCKET conn = CreateConnectionSocket();
    if (conn == SOCKET_ERROR)
        return;

    m_listenWorker = CreateListenWorker(conn);
    std::string input;

    do
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.size() > 0)
        {
            SendMsg(conn, input);
        }
    } while (input.size() > 0);

    closesocket(conn);
}

SOCKET Network::TcpConnecter::CreateConnectionSocket()
{
    SOCKET connSock = CreateSocket(AF_INET, SOCK_STREAM, 0);
    if (connSock == SOCKET_ERROR)
        return SOCKET_ERROR;

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

    int connResult = connect(connSock, (const sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        LOG_ERROR("Can't connect to server with error - " + std::to_string(WSAGetLastError()));
        closesocket(connSock);
        return SOCKET_ERROR;
    }

    return connSock;
}

std::thread Network::TcpConnecter::CreateListenWorker(SOCKET connectionSocket)
{
    return std::thread(m_listeningHandler, connectionSocket);
}
