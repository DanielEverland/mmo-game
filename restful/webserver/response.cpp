#include "response.h"

response::response(status_code in_status_code) : code(in_status_code)
{
}

response::operator std::shared_ptr<common::socket::message>()
{
    std::string raw = std::format("HTTP/1.1 {} {}\r\n", static_cast<int>(code), status_code_map[code]);

    auto iter = headers.begin();
    while (iter != headers.end())
    {
        raw += std::format("{}:{}", iter->first, iter->second);

        ++iter;
        if (iter != headers.end())
            raw += "\r\n";
    }

    raw += "\r\n";
    raw += content;

    return make_shared<common::socket::message>(raw);
}
