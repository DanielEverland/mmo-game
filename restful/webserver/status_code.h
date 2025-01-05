#pragma once
#include <format>
#include <memory>

#include "webserver.h"

enum class status_code
{
    OK = 200,

    BAD_REQUEST = 400,
    NOT_FOUND = 404,
};

inline std::map<status_code, std::string> status_code_map = {
    { status_code::OK, "Ok" },
    
    { status_code::BAD_REQUEST, "Bad Request" },
    { status_code::NOT_FOUND, "Not Found" }
};

inline shared_ptr<common::socket::message> create_message(status_code status_code)
{
    auto new_message = std::make_shared<common::socket::message>();

    std::string lit = std::format("HTTP/1.1 {}\n\n", static_cast<int>(status_code));
    new_message->data = new char[lit.length() + 1];
    std::ranges::copy(lit, new_message->data);
    new_message->size = lit.length();

    return new_message;
}