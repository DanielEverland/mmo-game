#pragma once
#include <sstream>

class string_utility
{
public:
    static bool try_parse_next_word(std::stringstream& in_stream, std::string& word);
    static bool try_parse_line(std::stringstream& in_stream, std::string& word);
    static bool try_parse_line_split(std::stringstream& in_stream, char deliminator, std::string& first, std::string& second);
    static void purge_line_feed(std::stringstream& in_stream);
    static void purge_whitespace(std::stringstream& in_stream);
};
