#ifndef HTTPS_SERVER_IMPL
#define HTTPS_SERVER_IMPL

#include "uri_parser.h"
#include "https_server.h"
#include "../httpparser/src/httpparser/urlparser.h"


template <typename T>
std::ostream & operator << (std::ostream & s, std::vector <T> const & v)
{
    for (T const & i : v)
    {
        s << i << " ";
    }
    return s;
}


template <typename T>
std::ostream & operator << (std::ostream & s, std::vector <std::vector <T> > const & v)
{
    for (std::vector <T> const & i : v)
    {
        s << i << "\n";
    }
    return s;
}



template <typename DB = DAO, typename TIME = timer>
struct https_server_impl : https_server
{
    https_server_impl 
    (
        std::atomic <bool> const & _stop, 
        std::string const & port
    ) :
        https_server(_stop, port),
        dao(std::make_shared <DB> ()),
        enter_v(dao),
        statistic_v(dao),
        manager_v(dao)
    {}

    virtual std::string handle (httpparser::Request const & request) noexcept
    {
        try
        {
            uri uri = parse_uri(url_decode(request.uri));
            std::map <std::string, std::string> params;
            std::string method = uri.path;
            
            for (auto i : uri.args)
            {
                params[i.key] = i.value;
                //std::cout << i.key << " " << i.value << "\n";
            }
        
            //std::cout << request.method << "\n";
            //std::cout << request.uri << "\n";
            //std::cout << request.content << "\n";
            if (method == "/create_abonement")
            {
                manager_v.create_abonement(abonement(params));
                return "ok";
            }
            else if (method == "/continue_abonement")
            {
                manager_v.continue_abonement(abonement_continue(params));
                return "ok";
            }
            else if (method == "/get_abonement_info")
            {
                int id = std::stoi(params["abonement_id"]);
                abonement answer = manager_v.get_abonement_info(id);
                return to_string(answer);
            }
            else if (method == "/enter")
            {
                int id = std::stoi(params["abonement_id"]);
                enter_v.entering(id);
                return "ok";
            }
            else if (method == "/get_statistic")
            {
                return statistic_v.get_stat();
            }
            else
            {
                return "unknown command: " + method;
            }
        }
        catch (std::exception const & e)
        {
            return std::string("error: ") + e.what();
        }
        catch (...)
        {
            return "unknown error";
        }
    }
    
private:
    std::shared_ptr <DB> dao;
    enter_t <DB, TIME> enter_v;
    statistic_t <DB, TIME> statistic_v;
    manager_t <DB, TIME> manager_v;
};


#endif
