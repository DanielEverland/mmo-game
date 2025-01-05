#pragma once
#include <map>
#include <string>

#include "status_code.h"
#include "network/socket.h"

class response
{
public:
    response(status_code in_status_code);
    
    status_code code;
    std::map<std::string, std::string> headers;
    std::string content;
    
    operator std::shared_ptr<common::socket::message>();
};
