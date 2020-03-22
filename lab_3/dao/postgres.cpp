#include "postgres.h"

pqxx::result DAO::get_all_abonement_continues_ (pqxx::transaction_base &txn)
{
    return txn.exec("SELECT abonement_id, use_to FROM abonement_continue order by operation_id");
}

pqxx::result DAO::get_abonement_continues_ (pqxx::transaction_base &txn, int abonement_id)
{
    return txn.exec
    (
        "SELECT abonement_id, use_to FROM abonement_continue where abonement_id = " + 
        txn.quote(abonement_id) + 
        " order by operation_id"
    );
}

std::vector <abonement_continue> DAO::get_all_abonement_continues ()
{
    return get_vector <abonement_continue> (get_all_abonement_continues_);
}

std::vector <abonement_continue> DAO::get_abonement_continues (int abonement_id)
{
    return get_vector <abonement_continue> (get_abonement_continues_, abonement_id);
}

void DAO::continue_abonement (pqxx::transaction_base &txn, abonement_continue const & abonement)
{
    txn.exec0
    (
        "INSERT INTO abonement_continue (abonement_id, use_to) "
        "VALUES (" +
        txn.quote(abonement.id) + ", " +
        txn.quote(abonement.use_to) +
        ")"
    );
}

void DAO::continue_abonement (abonement_continue const & abonement)
{
    retrieve <void (*) (pqxx::transaction_base &, abonement_continue const &)> (continue_abonement, abonement);
}








void DAO::subscribe (std::function <void (enter const &)> const & f)
{
    //std::cout << "subscriber\n";
    callbacks.push_back(f);
}


pqxx::result DAO::get_all_enter_events_ (pqxx::transaction_base &txn)
{
    return txn.exec("SELECT abonement_id, time FROM enter_info order by time");
}

pqxx::result DAO::get_enter_events_ (pqxx::transaction_base &txn, int abonement_id)
{
    return txn.exec
    (
        "SELECT abonement_id, time FROM enter_info where abonement_id = " + 
        txn.quote(abonement_id) + 
        " order by time"
    );
}

std::vector <enter> DAO::get_all_enter_events ()
{
    return get_vector <enter> (&DAO::get_all_enter_events_);
}

std::vector <enter> DAO::get_enter_events (int abonement_id)
{
    return get_vector <enter> (&DAO::get_enter_events_, abonement_id);
}

void DAO::entering_ (pqxx::transaction_base &txn, enter const & e)
{
    txn.exec0
    (
        "INSERT INTO enter_info (abonement_id, time) "
        "VALUES (" +
        txn.quote(e.id) + ", " +
        txn.quote(e.time) +
        ")"
    );

    for (std::function <void (enter const &)> const & f : callbacks)
    {
        f(e);
    }

}

void DAO::entering (enter const & e)
{
    retrieve(&DAO::entering_, e);
}










pqxx::result DAO::get_all_abonement_ (pqxx::transaction_base &txn)
{
    return txn.exec("SELECT abonement_id, client_name, use_to FROM abonement_info");
}

pqxx::result DAO::get_abonement_ (pqxx::transaction_base &txn, int abonement_id)
{
    return txn.exec
    (
        "SELECT abonement_id, client_name, use_to FROM abonement_info where abonement_id = " + 
        txn.quote(abonement_id)
    );
}

std::vector <abonement> DAO::get_all_abonement ()
{
    return get_vector <abonement> (get_all_abonement_);
}

abonement DAO::get_abonement (int abonement_id)
{
    pqxx::result res = retrieve(get_abonement_, abonement_id);
    if (res.empty())
    {
        throw std::runtime_error("has not abonement with id " + std::to_string(abonement_id));
    }
    return abonement(res[0]);
}

void DAO::create_abonement_ (pqxx::transaction_base &txn, abonement const & abonement)
{
    txn.exec0
    (
        "INSERT INTO abonement_info (abonement_id, client_name, use_to) "
        "VALUES (" +
        txn.quote(abonement.id) + ", " +
        txn.quote(abonement.client_name) + ", " +
        txn.quote(abonement.use_to) +
        ")"
    );
}

void DAO::create_abonement (abonement const & abonement)
{
    retrieve(&DAO::create_abonement_, abonement);
}




DAO::transaction::transaction () :
    conn(),
    txn{conn}
{}

pqxx::transaction_base & DAO::transaction::operator () ()
{
    return txn;
}

DAO::transaction::~transaction ()
{
    try
    {
        txn.commit();
    }
    catch (...)
    {}
}
        


