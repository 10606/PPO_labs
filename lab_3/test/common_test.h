#ifndef COMMON_TEST_H
#define COMMON_TEST_H

#include <atomic>
#include "../processing/processing.h"
#include "curl_wrap.h"

struct curl_ignore : public curl_wrap
{
    virtual std::stringstream get 
    (
        std::string const & request, 
        std::map <std::string, std::string> const & properties = {}
    );
};




struct stub_db
{
    void create_abonement(abonement const & a);
    void continue_abonement(abonement_continue const & a);
    abonement get_abonement(int id);
    std::vector <abonement_continue> get_abonement_continues (int id);
    std::vector <enter> get_all_enter_events ();
    void subscribe (std::function <void (enter const &)> const & f);
    void entering (enter const & e);
    void clear ();

    std::vector <std::function <void (enter const &)> > callbacks;
    std::map <int, abonement> abonements;
    std::map <int, std::vector <abonement_continue> > continues;
    std::vector <enter> enters;
};

struct stub_timer
{
    std::time_t get_time ();
    static std::atomic <std::time_t> value;
};



#endif 
