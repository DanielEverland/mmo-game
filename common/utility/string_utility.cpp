#include "string_utility.h"

#include <string>

bool string_utility::try_parse_next_word(std::stringstream& in_stream, std::string& word)
{
    static char word_buffer[64];

    int idx = 0;
    char curr;
    while (in_stream.read(&curr, 1))
    {
        if (curr == ' ' || curr == '\r')
        {
            purge_line_feed(in_stream);            
            word = std::string(word_buffer, idx);
            return true;
        }

        word_buffer[idx] = curr;
        idx++;
    }

    return false;
}

bool string_utility::try_parse_line(std::stringstream& in_stream, std::string& line)
{
    static char line_buffer[512];

    int idx = 0;
    char curr;
    while (in_stream.read(&curr, 1))
    {
        if (curr == '\n')
        {
            purge_line_feed(in_stream);
            line = std::string(line_buffer, idx);
            return true;
        }

        line_buffer[idx] = curr;
        idx++;
    }

    return false;
}

bool string_utility::try_parse_line_split(std::stringstream& in_stream, char deliminator, std::string& first, std::string& second)
{
    static char buffer[64];

    bool has_seen_delim = false;
    int idx = 0;
    char curr;
    while (in_stream.read(&curr, 1))
    {
        if (curr == '\r')
        {
            // Line does not contain deliminator
            if (!has_seen_delim)
            {
                buffer[idx] = curr;
                idx++;
                
                if (in_stream.peek() == '\n')
                {
                    in_stream.read(buffer + idx, 1);
                    idx++;
                }

                first = std::string(buffer, idx);
                return false;
            }

            second = std::string(buffer, idx);            
            purge_line_feed(in_stream);
            return true;
        }
        
        if (!has_seen_delim && curr == deliminator)
        {
            first = std::string(buffer, idx);
            idx = 0;
            has_seen_delim = true;
            purge_whitespace(in_stream);
        }
        else
        {
            buffer[idx] = curr;
            idx++;
        }
    }

    return true;
}

void string_utility::purge_line_feed(std::stringstream& in_stream)
{
    if (in_stream.peek() == '\n')
        in_stream.ignore();
}

void string_utility::purge_whitespace(std::stringstream& in_stream)
{
    while(in_stream.peek() == ' ')
        in_stream.ignore();
}
