#pragma once

#include "Socket.h"

#include <thread>

namespace Network
{
    class TcpConnecter;
    typedef void(*ListeningHandler)(int socketId);

    class TcpConnecter : public Socket
    {
    public:
        TcpConnecter(const std::string& ipAddress, int port, ListeningHandler handler);
        ~TcpConnecter();

        bool Init();

        void Run();

        std::thread& GetListenWorker() { return m_listenWorker; }

    private:
        // Creates listen socket
        // Returns SOCKET upon success or SOCKET_ERROR(-1) if any error occurs
        SOCKET CreateConnectionSocket();
        std::thread CreateListenWorker(SOCKET connectionSocket);

    private:
        std::string m_ipAddress;
        int m_port;
        ListeningHandler m_listeningHandler;
        std::thread m_listenWorker;
    };
}

