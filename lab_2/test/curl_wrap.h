#ifndef CURL_WRAP
#define CURL_WRAP

#include <curl/curl.h>
#include <exception>
#include <functional>
#include <sstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>

size_t write_to_sstream_callback (const char * input, size_t, size_t size, void * write);

typedef size_t (* write_func_signature) (const char *, size_t, size_t, void *);


struct code_and_slist
{
    CURLcode code;
    std::unique_ptr <curl_slist, void (*) (curl_slist *)> slist;
    
    operator CURLcode ();
};


class curl_wrap
{
public:
    curl_wrap 
    (
        std::function <void ()> global_init = 
            [] () -> void {curl_global_init(CURL_GLOBAL_DEFAULT);},
        std::function <CURL * ()> object_init = curl_easy_init,
        std::function <void ()> _global_destroy = curl_global_cleanup,
        std::function <void (CURL *)> _object_destroy = curl_easy_cleanup,
        std::function <CURLcode (CURL *, const char *)> _set_url = 
            [] (CURL * object, const char * url) -> CURLcode
            {return curl_easy_setopt(object, CURLOPT_URL, url);},
        std::function <CURLcode (CURL *, void *)> _set_data = 
            [] (CURL * object, void * data) -> CURLcode
            {return curl_easy_setopt(object, CURLOPT_WRITEDATA, data);},
        std::function <CURLcode (CURL *)> _set_write_func = 
            [] (CURL * object) -> CURLcode
            {return curl_easy_setopt(object, CURLOPT_WRITEFUNCTION, write_to_sstream_callback);},
        std::function <code_and_slist (CURL *, std::map <std::string, std::string> const & values)> _set_property = 
            [] (CURL * object, std::map <std::string, std::string> const & values) -> code_and_slist
            {
                struct curl_slist * header = nullptr;
                for (auto const & value : values)
                {
                    header = curl_slist_append(header, (value.first + ": " + value.second).c_str());
                }
                CURLcode ret = curl_easy_setopt(object, CURLOPT_HTTPHEADER, header);
                return code_and_slist{ret, std::unique_ptr <curl_slist, void (*) (curl_slist *)> (header, curl_slist_free_all)};
            },
        std::function <CURLcode (CURL *)> _run = curl_easy_perform
    );
    
    virtual ~curl_wrap ();
    
    curl_wrap (curl_wrap &&) = default;
    curl_wrap (curl_wrap const &) = delete;

    virtual std::stringstream get (std::string const & request, std::map <std::string, std::string> const & properties = {});
    
    template <typename T>
    void set_opt (CURLoption option_name, T const & a, std::function <CURLcode (CURL *, CURLoption, T)> setter = curl_easy_setopt)
    {
        ret_code_to_exception(setter(curl, option_name, a));
    }

private:
    void ret_code_to_exception (CURLcode code);
    
    void perform ();
    
    CURL * curl;
    std::function <void ()> global_destroy;
    std::function <void (CURL *)> object_destroy;
    std::function <CURLcode (CURL *, const char *)> set_url;
    std::function <CURLcode (CURL *, void *)> set_data;
    std::function <CURLcode (CURL *)> set_write_func;
    std::function <code_and_slist (CURL *, std::map <std::string, std::string> const & values)> set_property;
    std::function <CURLcode (CURL *)> run;
};

#endif
