#include "uri_parser.h"



uri parse_uri (std::string const & uri_in_str)
{
    uri ans;
    
    size_t path_end_index = uri_in_str.find_first_of('?');
    ans.path = uri_in_str.substr(0, path_end_index);

    if (path_end_index == std::string::npos)
    {
        return ans;
    }

    
    std::string tail(uri_in_str.substr(path_end_index + 1));

    while (1)
    {
        size_t pos = tail.find_first_of('&');
        std::string cur_arg = tail.substr(0, pos);
        
        size_t key_pos = cur_arg.find_first_of('=');
        if (key_pos != std::string::npos)
        {
            std::string key = cur_arg.substr(0, key_pos);
            std::string value = cur_arg.substr(key_pos + 1);
            ans.args.push_back({key, value});
        }
        
        if (pos == std::string::npos)
        {
            return ans;
        }
        tail = tail.substr(pos + 1);
    }
}


