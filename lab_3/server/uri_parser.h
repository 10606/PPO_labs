#ifndef URI_PARSER_H
#define URI_PARSER_H

#include <vector>
#include <string>

struct uri
{
    std::string path;
    
    struct argument
    {
        std::string key;
        std::string value;
    };
    
    std::vector <argument> args;
};

uri parse_uri (std::string const & uri_in_str);

#endif

