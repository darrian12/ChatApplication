#pragma once

#include <string>
#include <WS2tcpip.h>

#include "util.h"

namespace Network
{
    class Socket
    {
    public:
        Socket();
        ~Socket();

        bool InitWinsock(int vA, int vB);
        SOCKET CreateSocket(int family, int type, int protocol);
    };
}
