#include "Socket.h"

Network::Socket::Socket()
{
}

Network::Socket::~Socket()
{
}

bool Network::Socket::InitWinsock(int vA, int vB)
{
    WSAData data;
    WORD ver = MAKEWORD(vA, vB);

    int winSock = WSAStartup(ver, &data);
    if (winSock != 0)
    {
        LOG_ERROR("failed to start Winsock with error #" + std::to_string(winSock));
        return false;
    }

    return true;
}

SOCKET Network::Socket::CreateSocket(int family, int type, int protocol)
{
    SOCKET sock = socket(family, type, protocol);
    if (sock == INVALID_SOCKET)
    {
        LOG_ERROR("Failed to create socket with error #" + std::to_string(WSAGetLastError()));
        return SOCKET_ERROR;
    }

    return sock;
}
