#include <gtest/gtest.h>

#include "../processing/processing.h"
#include "../server/https_server.h"
#include "common_test.h"

TEST(processing, manager)
{
    std::shared_ptr <stub_db> db = std::make_shared <stub_db> ();
    manager_t <stub_db, stub_timer> manage(db);

    abonement a(1, "q", "e");
    abonement b(2, "s", "f");
    EXPECT_NO_THROW(manage.create_abonement(a));
    EXPECT_NO_THROW(manage.create_abonement(b));
    EXPECT_TRUE(db->abonements.at(1) == a);
    EXPECT_TRUE(db->abonements.at(2) == b);
    EXPECT_THROW(manage.create_abonement(a), std::runtime_error);

    abonement_continue c_0(1, "w");
    manage.continue_abonement(c_0);
    EXPECT_TRUE(manage.get_abonement_info(1) == abonement(1, "q", "w"));
    EXPECT_TRUE(manage.get_abonement_info(2) == abonement(2, "s", "f"));
    abonement_continue c_1(1, "t");
    manage.continue_abonement(c_1);
    EXPECT_FALSE(manage.get_abonement_info(1) == abonement(1, "q", "e"));
    EXPECT_FALSE(manage.get_abonement_info(1) == abonement(1, "q", "w"));
    EXPECT_TRUE(manage.get_abonement_info(1) == abonement(1, "q", "t"));
    abonement ans = manager_t <stub_db, stub_timer> ::get_abonement_info(2, db);
    EXPECT_TRUE(ans == abonement(2, "s", "f"));
    EXPECT_ANY_THROW(manage.get_abonement_info(3));
}

TEST(processing, enter)
{
    std::time_t t_0 = 0,
                t_1 = 2,
                t_2 = 4,
                t_3 = 6;
    std::string ts_0 = to_string(t_0),
                ts_1 = to_string(t_1),
                ts_2 = to_string(t_2),
                ts_3 = to_string(t_3);
    
    std::shared_ptr <stub_db> db = std::make_shared <stub_db> ();
    manager_t <stub_db, stub_timer> manage(db);

    abonement a(1, "q", ts_0);
    abonement b(2, "s", ts_2);
    manage.create_abonement(a);
    manage.create_abonement(b);
    manage.continue_abonement(abonement_continue(1, ts_1));

    enter_t <stub_db, stub_timer> enter(db);
    EXPECT_EQ(db->callbacks.size(), 0);
    stub_timer::value = t_0;
    EXPECT_NO_THROW(enter.entering(1));
    EXPECT_NO_THROW(enter.entering(2));
    stub_timer::value = t_1;
    EXPECT_NO_THROW(enter.entering(2));
    stub_timer::value = t_2;
    EXPECT_ANY_THROW(enter.entering(1));
    stub_timer::value = t_3;
    EXPECT_ANY_THROW(enter.entering(1));
    EXPECT_ANY_THROW(enter.entering(2));
}


TEST(processing, statistic)
{
    std::time_t t_0 = 0 * 60,
                t_1 = 2 * 60,
                t_2 = 4 * 60,
                t_3 = 6 * 60;
    std::string ts_0 = to_string(t_0),
                ts_1 = to_string(t_1),
                ts_2 = to_string(t_2),
                ts_3 = to_string(t_3);
    
    std::time_t t_10 = 0 * 60 + 24 * 3600 * 2,
                t_11 = 2 * 60 + 24 * 3600 * 2,
                t_12 = 4 * 60 + 24 * 3600 * 2,
                t_13 = 6 * 60 + 24 * 3600 * 2;
    std::string ts_10 = to_string(t_10),
                ts_11 = to_string(t_11),
                ts_12 = to_string(t_12),
                ts_13 = to_string(t_13);
    
    std::shared_ptr <stub_db> db = std::make_shared <stub_db> ();
    manager_t <stub_db, stub_timer> manage(db);

    abonement a(1, "q", ts_13);
    abonement b(2, "s", ts_13);
    manage.create_abonement(a);
    manage.create_abonement(b);
    
    statistic_t <stub_db, stub_timer> stat_0(db);
    enter_t <stub_db, stub_timer> enter(db);
    stub_timer::value = t_0;
    EXPECT_NO_THROW(enter.entering(1));
    stub_timer::value = t_1;
    EXPECT_NO_THROW(enter.entering(2));
    stub_timer::value = t_2;
    EXPECT_NO_THROW(enter.entering(2));

    statistic_t <stub_db, stub_timer> stat_1(db);
    stub_timer::value = t_3;
    EXPECT_NO_THROW(enter.entering(1));

    
    
    
    statistic_t <stub_db, stub_timer> stat_2(db);
    stub_timer::value = t_10;
    EXPECT_NO_THROW(enter.entering(1));
    stub_timer::value = t_11;
    EXPECT_NO_THROW(enter.entering(2));
    
    statistic_t <stub_db, stub_timer> stat_3(db);
    stub_timer::value = t_12;
    EXPECT_NO_THROW(enter.entering(1));

    EXPECT_EQ(stat_0.get_stat(), stat_1.get_stat());
    EXPECT_EQ(stat_0.get_stat(), stat_2.get_stat());
    EXPECT_EQ(stat_0.get_stat(), stat_3.get_stat());
    EXPECT_EQ(stat_0.get_stat(), "duration  = 4.000000\nfrequency = 2.000000\n1970-01-01 03:00:00 2.000000\n1970-01-03 03:00:00 2.000000\n");
}

bool operator == (std::tm const & a, std::tm const & b)
{
    return  a.tm_sec  == b.tm_sec  &&
            a.tm_min  == b.tm_min  &&
            a.tm_hour == b.tm_hour &&
            a.tm_mday == b.tm_mday &&
            a.tm_mon  == b.tm_mon  &&
            a.tm_year == b.tm_year;
            /*a.tm_yday == b.tm_yday; //does not work(((*/
}

TEST(time_convert, base)
{
    std::string ts_0 = "1970-01-01 03:00:00"; //GMT+3
    std::tm lc_0 = get_local_time(0);
    std::tm ls_0 = get_local_time(ts_0);
    std::string s_0 = to_string(0);
    std::time_t t_0 = get_time(ts_0);

    EXPECT_EQ(lc_0, ls_0);
    EXPECT_EQ(s_0, ts_0);
    EXPECT_EQ(0, t_0);

    std::string ts_1 = "1970-01-01 03:00:01";
    std::tm lc_1 = get_local_time(1);
    std::tm ls_1 = get_local_time(ts_1);
    std::string s_1 = to_string(1);
    std::time_t t_1 = get_time(ts_1);

    EXPECT_EQ(lc_1, ls_1);
    EXPECT_EQ(s_1, ts_1);
    EXPECT_EQ(1, t_1);

    std::time_t cur = std::time(nullptr);
    std::tm lc_2 = get_local_time(cur);
    std::string s_2 = to_string(cur);
    std::tm ls_2 = get_local_time(s_2);
    std::time_t t_2 = get_time(s_2);

    EXPECT_EQ(lc_2, ls_2);
    EXPECT_EQ(cur, t_2);
}

TEST(url_decode, base)
{
    std::string dec;
    dec = url_decode("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000:00:00");
    EXPECT_EQ(dec, "https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03 00:00:00");

    dec = url_decode("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000%2000:00");
    EXPECT_EQ(dec, "https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03 00 00:00");

    dec = url_decode("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000%2000:00%20");
    EXPECT_EQ(dec, "https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03 00 00:00 ");

    EXPECT_NO_THROW(url_decode("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000%2000:00%"));
}



