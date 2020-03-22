#ifndef TIME_CONVERT_H
#define TIME_CONVERT_H

#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

std::tm get_local_time (std::time_t time);
std::tm get_local_time (std::string const & time);
std::string to_string (std::time_t time);
std::time_t get_time (std::string const & time);

struct timer
{
    static std::time_t get_time ();
};

#endif

