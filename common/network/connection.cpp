#include "connection.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

#include "socket.h"

using namespace common;
using std::shared_ptr;

void connection::create(common::socket* in_owner, SOCKET socket_ptr)
{
    assert(in_owner != nullptr);
    assert(socket_ptr != INVALID_SOCKET);
    
    all_connections.push_back(connection(in_owner, socket_ptr));
    connection* conn = &all_connections.back();
    std::thread thread(&connection::handle_connection_async, conn, socket_ptr);
    thread.detach();
}

connection::connection(common::socket* in_owner, SOCKET in_socket) : owner(in_owner), socket(in_socket)
{
    
}

void connection::send_message(shared_ptr<common::socket::message> msg)
{
    int iSendResult = send(socket, msg->data, msg->size, 0);
    if (iSendResult == SOCKET_ERROR)
    {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }
    assert(iSendResult != SOCKET_ERROR);
}

void connection::close()
{
    closesocket(socket);
    is_active_ = false;
}

void connection::handle_connection_async(SOCKET socket_ptr)
{
    std::cout << "Handling " << socket_ptr << " on thread " << std::this_thread::get_id() << '\n';

    int constexpr buffer_size = common::socket::message_buffer_size;
    char receiveBuffer[buffer_size];


    fd_set socket_fd;
    FD_ZERO(&socket_fd);
    FD_SET(socket_ptr, &socket_fd);
    
    while (is_active_)
    {
        assert(owner != nullptr);
        
        int result = recv(socket_ptr, receiveBuffer, buffer_size, 0);
        if (result > 0)
        {
            std::cout << "Bytes received: " << result << '\n';

            auto new_message = std::make_shared<common::socket::message>();
            new_message->data = new char[result];
            memcpy(new_message->data, receiveBuffer, result);
            new_message->size = result;
            std::cout << new_message->data << '\n';
            
            owner->on_received_message(new_message, this);
        }
    }
}
