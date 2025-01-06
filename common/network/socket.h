#pragma once

#if _WIN32
#include <winsock2.h>
#endif
#include <functional>
#include <mutex>
#include <queue>

using std::shared_ptr;

namespace common
{
    class connection;
    
    class socket : public std::enable_shared_from_this<socket>
    {
    public:
        struct message
        {
            message() = default;
            explicit message(const std::string& message);
            ~message();            
            
            char* data;
            size_t size;
        };
        
        static constexpr int message_buffer_size = 512;
        
        static inline std::function<void(shared_ptr<connection>, shared_ptr<message>, SOCKET)> message_callbacks = { };
        
        socket(int port);

        void tick();
        SOCKET get_socket() const { return ListenSocket; }
        
        void on_received_message(shared_ptr<message> new_message, shared_ptr<connection> conn, SOCKET socket_ptr);
        
    private:
#if _WIN32
        static inline WSADATA wsaData = {};

        SOCKET ListenSocket;
    
        static inline auto initialized_winsock = false;
        static void initialize_winsock();
#endif
    };
}
