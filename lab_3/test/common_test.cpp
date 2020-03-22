#include "common_test.h"



std::stringstream curl_ignore::get 
(
    std::string const & request, 
    std::map <std::string, std::string> const & properties
)
{
    set_opt(CURLOPT_SSL_VERIFYPEER, 0L);
    return curl_wrap::get(request, properties);
}





void stub_db::create_abonement(abonement const & a)
{
    if (abonements.find(a.id) != abonements.end())
    {
        throw std::runtime_error("already exist");
    }
    abonements.insert({a.id, a});
}

void stub_db::continue_abonement(abonement_continue const & a)
{
    if (abonements.find(a.id) == abonements.end())
    {
        throw std::runtime_error("has not abonement");
    }
    continues[a.id].push_back(a);
}

abonement stub_db::get_abonement(int id)
{
    return abonements.at(id);
}

std::vector <abonement_continue> stub_db::get_abonement_continues (int id)
{
    if (continues.find(id) == continues.end())
    {
        return {};
    }
    else
    {
        return continues.at(id);
    }
}

std::vector <enter> stub_db::get_all_enter_events ()
{
    return enters;
}


void stub_db::subscribe (std::function <void (enter const &)> const & f)
{
    callbacks.push_back(f);
}

void stub_db::entering (enter const & e)
{
    if (abonements.find(e.id) == abonements.end())
    {
        throw std::runtime_error("has not abonement");
    }
    for (std::function <void (enter const &)> const & f : callbacks)
    {
        f(e);
    }
    enters.push_back(e);
}

void stub_db::clear ()
{
    callbacks.clear();
    abonements.clear();
    continues.clear();
    enters.clear();
}


std::time_t stub_timer::get_time ()
{
    return value.load();
}
    
std::atomic <std::time_t> stub_timer::value;


