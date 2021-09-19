#pragma once

#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#include "Socket.h"
#include "util.h"

#define MAX_BUFFER_SIZE (4096)

namespace Network
{
    class TcpListener;
    typedef void (*MessageReceivedHandler)(TcpListener* listener, int clientSocket, int senderSocket, const std::string& msg);

    class TcpListener : public Socket
    {
    public:
        TcpListener(const std::string& ipAddress, int port, MessageReceivedHandler handler);
        ~TcpListener();

        bool Init();

        void Run();

    private:
        // Creates listen socket
        // Returns SOCKET upon success or SOCKET_ERROR(-1) if any error occurs
        SOCKET CreateListenSocket();
        // Creates client socket
        // Returns SOCKET upon success or SOCKET_ERROR(-1) if any error occurs
        SOCKET WaitForConnection(SOCKET listenSocket, sockaddr* addr, int* addrlen);

    private:
        std::string m_ipAddress;
        int m_port;
        MessageReceivedHandler m_messageReceivedHandler;
    };
}
