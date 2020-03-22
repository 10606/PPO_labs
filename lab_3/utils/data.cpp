#include "data.h"



abonement::abonement (pqxx::row const & row) :
    abonement
    (
        row["abonement_id"].as <int> (), 
        row["client_name"].as <std::string> (), 
        row["use_to"].as <std::string> ()
    )
{}

abonement::abonement 
(
    int _id, 
    std::string const & _client_name, 
    std::string const & _use_to
) :
    id(_id),
    client_name(_client_name),
    use_to(_use_to)
{}

abonement::abonement (std::map <std::string, std::string> const & params) :
    abonement
    (
        std::stoi(params.at("abonement_id")),
        params.at("client_name"),
        params.at("use_to")
    )
{}




std::string to_string (abonement const & value)
{
    return std::to_string(value.id) + " " + value.client_name + " -> " + value.use_to;
}






abonement_continue::abonement_continue (pqxx::row const & row) :
    abonement_continue
    (
        row["abonement_id"].as <int> (), 
        row["use_to"].as <std::string> ()
    )
{}

abonement_continue::abonement_continue 
(
    int _id, 
    std::string const & _use_to
) :
    id(_id),
    use_to(_use_to)
{}

abonement_continue::abonement_continue (std::map <std::string, std::string> const & params) :
    abonement_continue
    (
        std::stoi(params.at("abonement_id")),
        params.at("use_to")
    )
{}






enter::enter (pqxx::row const & row) :
    enter
    (
        row["abonement_id"].as <int> (), 
        row["time"].as <std::string> ()
    )
{}

enter::enter 
(
    int _id, 
    std::string const & _time
) :
    id(_id),
    time(_time)
{}















bool operator == (abonement const & lhs, abonement const & rhs)
{
    return  lhs.id == rhs.id &&
            lhs.client_name == rhs.client_name &&
            lhs.use_to == rhs.use_to;
}

bool operator == (abonement_continue const & lhs, abonement_continue const & rhs)
{
    return  lhs.id == rhs.id &&
            lhs.use_to == rhs.use_to;
}

bool operator == (enter const & lhs, enter const & rhs)
{
    return  lhs.id == rhs.id &&
            lhs.time == rhs.time;
}







