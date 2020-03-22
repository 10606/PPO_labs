#ifndef POSTGRES_H
#define POSTGRES_H

#include <iostream>
#include <pqxx/pqxx>
#include <pqxx/pqxx>

#include <functional>
#include <vector>
#include "../utils/data.h"

template <typename CALL, typename RET>
using call = std::enable_if_t 
<
    std::is_same_v 
    <
        std::result_of_t <CALL>, 
        RET
    >, 
    int
>;


template <typename CALL, typename RET>
using not_call = std::enable_if_t 
<
    !std::is_same_v 
    <
        std::result_of_t <CALL>, 
        RET
    >, 
    int
>;

struct DAO
{

    template 
    <
        typename T
    >
    static std::vector <T>
    get_vector 
    (
        pqxx::result const & result
    )
    {
        std::vector <T> ans;
        for (auto i : result)
        {
            ans.push_back(T(i));
        }
        return ans;
    }


    template 
    <
        typename T,
        typename F, 
        typename ... Args,
        call <F(DAO*, pqxx::transaction_base &, Args ...), pqxx::result> = 5
    >
    std::vector <T> get_vector (F f, Args && ... args)
    {
        return get_vector <T> (retrieve((this->*f), std::forward <Args> (args) ...));
    }

    template 
    <
        typename T,
        typename F, 
        typename ... Args,
        call <F(pqxx::transaction_base &, Args ...), pqxx::result> = 5
    >
    static std::vector <T> get_vector (F f, Args && ... args)
    {
        return get_vector <T> (retrieve(f, std::forward <Args> (args) ...));
    }


    template 
    <
        typename T,
        typename F, 
        typename ... Args,
        call <F(DAO*, pqxx::transaction_base &, Args ...), pqxx::result> = 5
    >
    std::vector <T> get_vector (F f, pqxx::transaction_base & txn, Args && ... args)
    {
        return get_vector <T> ((this->*f)(txn, std::forward <Args> (args) ...));
    }

    template 
    <
        typename T,
        typename F, 
        typename ... Args,
        call <F(pqxx::transaction_base &, Args ...), pqxx::result> = 5
    >
    static std::vector <T> get_vector (F f, pqxx::transaction_base & txn, Args && ... args)
    {
        return get_vector <T> (f(txn, std::forward <Args> (args) ...));
    }



    static pqxx::result get_all_abonement_continues_ (pqxx::transaction_base &txn);
    static pqxx::result get_abonement_continues_ (pqxx::transaction_base &txn, int abonement_id);
    static std::vector <abonement_continue> get_all_abonement_continues ();
    static std::vector <abonement_continue> get_abonement_continues (int abonement_id);
    static void continue_abonement (pqxx::transaction_base &txn, abonement_continue const & abonement);
    static void continue_abonement (abonement_continue const & abonement);






    std::vector <std::function <void (enter const &)> > callbacks;

    void subscribe (std::function <void (enter const &)> const & f);;
    static pqxx::result get_all_enter_events_ (pqxx::transaction_base &txn);
    static pqxx::result get_enter_events_ (pqxx::transaction_base &txn, int abonement_id);
    static std::vector <enter> get_all_enter_events ();
    static std::vector <enter> get_enter_events (int abonement_id);
    void entering_ (pqxx::transaction_base &txn, enter const & e);
    void entering (enter const & e);







    static pqxx::result get_all_abonement_ (pqxx::transaction_base &txn);
    static pqxx::result get_abonement_ (pqxx::transaction_base &txn, int abonement_id);
    static std::vector <abonement> get_all_abonement ();
    static abonement get_abonement (int abonement_id);
    static void create_abonement_ (pqxx::transaction_base &txn, abonement const & abonement);
    static void create_abonement (abonement const & abonement);

    struct transaction
    {
        transaction ();
        pqxx::transaction_base & operator () ();
        ~transaction ();
        
    private:
        pqxx::connection conn;
        pqxx::work txn;
    };

    template 
    <
        typename F, 
        typename ... Args, 
        not_call <F(pqxx::transaction_base &, Args ...), void> = 5
    >
    static std::result_of_t <F(pqxx::transaction_base &, Args ...)> retrieve (F f, Args && ... args)
    {
        transaction transaction;
        return f(transaction(), std::forward <Args> (args) ...);
    }

    template 
    <
        typename F, 
        typename ... Args, 
        call <F(pqxx::transaction_base &, Args ...), void> = 5
    >
    static void retrieve (F f, Args && ... args)
    {
        transaction transaction;
        f(transaction(), std::forward <Args> (args) ...);
    }




    template 
    <
        typename F, 
        typename ... Args, 
        not_call <F(DAO *, pqxx::transaction_base &, Args ...), void> = 5
    >
    std::result_of_t <F(pqxx::transaction_base &, Args ...)> retrieve (F f, Args && ... args)
    {
        transaction transaction;
        return (this->*f)(transaction(), std::forward <Args> (args) ...);
    }

    template 
    <
        typename F, 
        typename ... Args, 
        call <F(DAO *, pqxx::transaction_base &, Args ...), void> = 5
    >
    void retrieve (F f, Args && ... args)
    {
        transaction transaction;
        (this->*f)(transaction(), std::forward <Args> (args) ...);
    }

};


#endif
