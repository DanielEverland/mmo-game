#include "request.h"

#include <iostream>
#include <memory>
#include <sstream>

#include "utility/string_utility.h"

bool request::try_parse(shared_ptr<common::socket::message> in_message, std::shared_ptr<request> out_request)
{    
    std::string full_raw = in_message->data;
    std::stringstream stream(full_raw);

    // Parse method
    std::string method_str;
    if (!string_utility::try_parse_next_word(stream, method_str))
        return false;
    out_request->method_type_ = to_method(method_str);

    // Parse path
    if (!string_utility::try_parse_next_word(stream, out_request->path))
        return false;

    if (out_request->path[0] != '/')
        return false;

    // Dump rest of line
    char dump;
    while (stream.read(&dump, 1) && dump != '\n') { }

    // Read headers
    std::string name;
    std::string value;
    do
    {
        if (string_utility::try_parse_line_split(stream, ':', name, value))
        {
            out_request->headers.insert(make_pair(name, value));
        }
        else if (name == "\r\n")
        {
            break;
        }
        else // something unexpected went wrong during parsing
        {
            return false;
        }
    }
    while (true);

    // Read content
    if (!out_request->headers.contains("Content-Length"))
    {
        // No content
        return true;
    }
    
    static char content_buffer[512];
    int content_length = std::stoi(out_request->headers["Content-Length"]);
    if (!stream.read(content_buffer, content_length))
        return false;
    out_request->content = std::string(content_buffer, content_length);
    
    return true;
}
