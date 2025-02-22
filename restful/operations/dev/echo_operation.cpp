﻿#include "echo_operation.h"

#include "webserver/request.h"
#include "webserver/response.h"
#include "webserver/status_code.h"

echo_operation::echo_operation() : operation(GET, "/dev/echo")
{
}

void echo_operation::execute(shared_ptr<common::connection> conn, shared_ptr<request> request, SOCKET socket_ptr)
{
    auto message = response(status_code::OK);
    message.content = request->get_content();
    
    conn->send_message(message, socket_ptr);
}
