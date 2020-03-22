
#include <curl/curl.h>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "curl_wrap.h"

size_t write_to_sstream_callback (const char * input, size_t, size_t size, void * write) 
{
    reinterpret_cast <std::stringstream *> (write) -> write(input, size); 
    return size;
}

typedef size_t (* write_func_signature) (const char *, size_t, size_t, void *);

code_and_slist::operator CURLcode ()
{
    return code;
}


curl_wrap::curl_wrap 
(
    std::function <void ()> global_init,
    std::function <CURL * ()> object_init,
    std::function <void ()> _global_destroy,
    std::function <void (CURL *)> _object_destroy,
    std::function <CURLcode (CURL *, const char *)> _set_url,
    std::function <CURLcode (CURL *, void *)> _set_data,
    std::function <CURLcode (CURL *)> _set_write_func,
    std::function <code_and_slist (CURL *, std::map <std::string, std::string> const & values)> _set_property,
    std::function <CURLcode (CURL *)> _run
) :
    curl(nullptr),
    global_destroy(_global_destroy),
    object_destroy(_object_destroy),
    set_url(_set_url),
    set_data(_set_data),
    set_write_func(_set_write_func),
    set_property(_set_property),
    run(_run)
{
    global_init();
    curl = object_init();

    if (curl == nullptr)
    {
        global_destroy();
        throw std::runtime_error("can't create curl object");
    }
}

curl_wrap::~curl_wrap ()
{
    object_destroy(curl);
    global_destroy();
}

std::stringstream curl_wrap::get (std::string const & request, std::map <std::string, std::string> const & properties)
{
    std::stringstream response;

    ret_code_to_exception(set_url(curl, request.c_str()));
    ret_code_to_exception(set_data(curl, &response));
    ret_code_to_exception(set_write_func(curl));
    code_and_slist header = set_property(curl, properties);
    ret_code_to_exception(header);

    ret_code_to_exception(run(curl));
    
    return response;
}

void curl_wrap::ret_code_to_exception (CURLcode code)
{
    if (code != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(code));
    }
}

void curl_wrap::perform ()
{
    CURLcode res;
    if ((res = curl_easy_perform(curl)) != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(res));
    }
}


