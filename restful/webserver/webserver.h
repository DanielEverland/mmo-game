#pragma once

#include <map>
#include <thread>

#include "operation.h"
#include "network/connection.h"

class webserver
{
public:
    webserver(int in_port);

    void add_operation(shared_ptr<operation> operation);

private:
    void tick();
    void handle_request(shared_ptr<common::connection> conn, std::shared_ptr<common::socket::message> msg, SOCKET socket_ptr);

    std::map<std::string, shared_ptr<operation>> operations;
    std::thread worker;
    int port;
};
