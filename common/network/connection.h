#pragma once
#include <memory>
#include <vector>
#include <winsock2.h>

#include "socket.h"

namespace common
{    
    class connection
    {
    public:
        explicit connection(socket* in_owner, SOCKET in_socket);

        void send_message(std::shared_ptr<socket::message> msg);
        void close();
    
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
        SOCKET socket;

        // Thread will die when this is false
        bool is_active_ = true;
    
        static inline std::vector<connection> all_connections = {};
    };
}
