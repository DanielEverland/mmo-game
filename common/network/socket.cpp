#include "socket.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "connection.h"


#if _WIN32
#undef UNICODE

#include <winsock2.h>
#include <ws2tcpip.h>

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace common
{
    socket::message::message(const std::string& message)
    {
        data = new char[message.size()];
        memcpy(data, message.c_str(), message.size());
        size = message.size();
    }

    socket::message::~message()
    {
        delete[] data;
    }

    socket::socket(int port)
    {
#if _WIN32
        if (!initialized_winsock)
        {
            initialize_winsock();
        }

        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory(&hints, sizeof (hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        int iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
        assert(iResult == 0);

        ListenSocket = INVALID_SOCKET;
        ListenSocket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        assert(ListenSocket != INVALID_SOCKET);

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        assert(iResult != SOCKET_ERROR);
        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        assert(iResult != SOCKET_ERROR);
#endif
    }

    void common::socket::tick()
    {
#if _WIN32
        SOCKET ClientSocket = INVALID_SOCKET;

        // Accept a client socket
        fd_set socket = { };
        FD_ZERO(&socket);
        FD_SET(ListenSocket, &socket);

        timeval timeout = { .tv_usec = 1 };    
        int iState = select(0, &socket, nullptr, nullptr, &timeout);
        assert(iState != SOCKET_ERROR);

        if (iState > 0)
        {
            ClientSocket = accept(ListenSocket, NULL, NULL);
            assert(ClientSocket != INVALID_SOCKET);
            
            if (ClientSocket > 0)
            {
                std::cout << "Client connected on " << ClientSocket << '\n';
                connection::create(this, ClientSocket);
            }
        }
#endif
    }

    void socket::on_received_message(shared_ptr<message> new_message, connection* conn, SOCKET socket_ptr)
    {
        message_callbacks(conn, new_message, socket_ptr);
    }

#if _WIN32
    void socket::initialize_winsock()
    {
        initialized_winsock = true;

        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        assert(iResult == 0);
    }
#endif
}