#pragma once

#include "webserver/operation.h"

class echo_operation : public operation
{
public:
    echo_operation();

    void execute(common::connection* conn, ::std::shared_ptr<request> request) override;
};
