#include "time_convert.h"

#include <iostream>

std::tm get_local_time (std::time_t time)
{
    std::tm local_time = *std::localtime(&time);
    return local_time;
}

std::tm get_local_time (std::string const & time)
{
    std::stringstream time_(time);
    std::tm local_time = {};
    time_ >> std::get_time(&local_time, "%Y-%m-%d %T");
    return local_time;
}

std::string to_string (std::time_t time)
{
    std::stringstream time_;
    //time_ << std::put_time(std::localtime(&time), "%d.%m.%Y");
    time_ << std::put_time(std::localtime(&time), "%Y-%m-%d %T");
    //std::cout << "<- time " << time_.str() << "\n";
    return time_.str();
}

std::time_t get_time (std::string const & time)
{
    //std::cout << "-> time " << time << "\n";
    std::stringstream time_(time);
    std::tm local_time = {};
    time_ >> std::get_time(&local_time, "%Y-%m-%d %T");
    return std::mktime(&local_time);
}

std::time_t timer::get_time ()
{
    return std::time(nullptr);
}
