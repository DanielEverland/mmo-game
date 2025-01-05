#include "webserver.h"

#include <iostream>
#include <thread>

#include "request.h"
#include "response.h"
#include "status_code.h"
#include "network/connection.h"
#include "network/socket.h"
#include "webserver/operation.h"

using message = common::socket::message;

webserver::webserver(int in_port)
{
    port = in_port;
    worker = std::thread(&webserver::tick, this);
}

void webserver::tick()
{
    std::cout << "Creating socket on thread " << std::this_thread::get_id() << '\n';
    auto serverSocket = common::socket(8080);
    serverSocket.message_callbacks = std::bind(&webserver::handle_request, this, std::placeholders::_1, std::placeholders::_2);
    
    while (true)
    {
        serverSocket.tick();
    }
}

void webserver::handle_request(common::connection* conn, std::shared_ptr<common::socket::message> msg)
{
    shared_ptr<request> new_request = std::make_shared<request>();
    bool succeeded = request::try_parse(msg, new_request);
    if (!succeeded)
    {
        conn->send_message(response(status_code::BAD_REQUEST));
        conn->close();
        return;
    }

    if (!operations.contains(new_request->get_path()))
    {
        conn->send_message(response(status_code::NOT_FOUND));
        conn->close();
        return;
    }

    operations[new_request->get_path()]->execute(conn, new_request);
    conn->close();
}

void webserver::add_operation(shared_ptr<operation> operation)
{
    operations.insert_or_assign(operation->get_path(), operation);
}
