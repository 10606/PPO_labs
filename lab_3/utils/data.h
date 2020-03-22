#ifndef DATA_H
#define DATA_H

#include <map>
#include <pqxx/pqxx>
#include <string>

struct abonement
{
    abonement (pqxx::row const &);
    abonement (int _id, std::string const & _client_name, std::string const & _use_to);
    abonement (std::map <std::string, std::string> const &);

    int id;
    std::string client_name;
    std::string use_to; //string because  input from user ---???--> DB --format--> code
};


std::string to_string (abonement const &);

struct abonement_continue
{
    abonement_continue (pqxx::row const &);
    abonement_continue (int _id, std::string const & _use_to);
    abonement_continue (std::map <std::string, std::string> const &);

    int id;
    std::string use_to;
};


struct enter
{
    enter (pqxx::row const &);
    enter (int _id, std::string const & _time);

    int id;
    std::string time;
};



bool operator == (abonement const & lhs, abonement const & rhs);
bool operator == (abonement_continue const & lhs, abonement_continue const & rhs);
bool operator == (enter const & lhs, enter const & rhs);


#endif
