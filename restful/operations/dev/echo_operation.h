#pragma once

#include "webserver/operation.h"

class echo_operation : public operation
{
public:
    echo_operation();

    void execute(shared_ptr<common::connection> conn, shared_ptr<request> request, SOCKET socket_ptr) override;
};
