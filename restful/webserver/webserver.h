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
    void handle_request(common::connection* conn, std::shared_ptr<common::socket::message> msg);

    std::map<std::string, shared_ptr<operation>> operations;
    std::thread worker;
    int port;
};
