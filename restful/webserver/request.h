#pragma once
#include <map>
#include <memory>

#include "operation.h"
#include "network/connection.h"

class request
{
public:
    static bool try_parse(shared_ptr<common::socket::message> in_message, std::shared_ptr<request> out_request);

    method get_method() const { return method_type_; }
    std::string get_path() const { return path; }
    [[nodiscard]] const std::string& get_content() const { return content; } 

private:
    method method_type_ = NONE;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string content;
};
