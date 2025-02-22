﻿#pragma once

#include <memory>
#include <string>
#include "network/socket.h"

class request;

enum method
{
    NONE = 0,
    
    GET,
};

inline method to_method(const std::string& str)
{
    if (str == "GET")
        return GET;

    return NONE;
}

class operation
{
public:
    virtual ~operation() = default;
    operation() = default;
    
    operation(method in_method, const std::string& in_path);

    std::string get_path() const { return path; }
    method get_method() const { return method; }

    virtual void execute(shared_ptr<common::connection> conn, shared_ptr<request> request, SOCKET socket_ptr) { }

private:
    std::string path;
    method method;
};
