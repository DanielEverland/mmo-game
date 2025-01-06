#pragma once
#include <memory>
#include <vector>
#include <winsock2.h>

#include "socket.h"

namespace common
{    
    class connection : public std::enable_shared_from_this<connection>
    {
    public:
        explicit connection(socket* in_owner);

        void send_message(std::shared_ptr<socket::message> msg, SOCKET socket_ptr);
        void close(SOCKET socket_ptr);
    
#if _WIN32
        static void create(socket* in_owner, SOCKET socket_ptr);

    private:
        void handle_connection_async(SOCKET socket_ptr);
    
    public:
#endif

    protected:
        virtual void handle_request(shared_ptr<socket::message> msg) { }

    private:
        socket* owner;

        // Thread will die when this is false
        bool is_active_ = true;
    };
}
