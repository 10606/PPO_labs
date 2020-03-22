#include "common_test.h"
#include "../server/https_server.h"
#include "../server/https_server_impl.h"
#include "../dao/postgres.h"
#include <gtest/gtest.h>

TEST(with_stub_dao, abonement)
{
    server_starter <https_server_impl <stub_db, stub_timer> > server("8080");
    curl_ignore curl;

    std::stringstream ss;
    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=1&use_to=20.03.2020");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8080/create_abonement?abonement_id=1&client_name=wa28&use_to=19.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=1");
    EXPECT_EQ(ss.str(), "1 wa28 -> 19.03.2020");
    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=1&use_to=20.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/create_abonement?abonement_id=1&client_name=wa28&use_to=19.03.2020");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=1&use_to=21.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=1");
    EXPECT_EQ(ss.str(), "1 wa28 -> 21.03.2020");

    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=2&use_to=20.03.2020");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8080/create_abonement?abonement_id=2&client_name=wa51&use_to=22.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=2");
    EXPECT_EQ(ss.str(), "2 wa51 -> 22.03.2020");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=1");
    EXPECT_EQ(ss.str(), "1 wa28 -> 21.03.2020");
    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=2&use_to=23.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/create_abonement?abonement_id=2&client_name=wa28&use_to=19.03.2020");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=1");
    EXPECT_EQ(ss.str(), "1 wa28 -> 21.03.2020");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=2");
    EXPECT_EQ(ss.str(), "2 wa51 -> 23.03.2020");

    ss = curl.get("https://127.0.0.1:8080/continue_abonement?abonement_id=1&use_to=25.03.2020");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=1");
    EXPECT_EQ(ss.str(), "1 wa28 -> 25.03.2020");
    ss = curl.get("https://127.0.0.1:8080/get_abonement_info?abonement_id=2");
    EXPECT_EQ(ss.str(), "2 wa51 -> 23.03.2020");
}

TEST(with_stub_dao, enter)
{
    server_starter <https_server_impl <stub_db, stub_timer> > server("8181");
    curl_ignore curl;

    std::stringstream ss;
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=1&client_name=wa28&use_to=1970-01-01%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-02%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-03%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=2&client_name=wa51&use_to=1970-01-02%2000:00:00");

    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=3");
    EXPECT_FALSE(ss.str() == "ok");

    stub_timer::value = 0;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_EQ(ss.str(), "ok");

    stub_timer::value = 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_FALSE(ss.str() == "ok");
    
    stub_timer::value = 2 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_FALSE(ss.str() == "ok");
    
    stub_timer::value = 3 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_FALSE(ss.str() == "ok");

    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-04%2000:00:00");

    stub_timer::value = 1 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_EQ(ss.str(), "ok");
    
    stub_timer::value = 2 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    EXPECT_EQ(ss.str(), "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    EXPECT_FALSE(ss.str() == "ok");

}




TEST(with_stub_dao, stat)
{
    server_starter <https_server_impl <stub_db, stub_timer> > server("8181");
    curl_ignore curl;

    std::stringstream ss;
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=1&client_name=wa28&use_to=1970-01-01%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-02%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-03%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=2&client_name=wa51&use_to=1970-01-02%2000:00:00");

    stub_timer::value = 0;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");

    stub_timer::value = 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2"); //fail
    
    stub_timer::value = 2 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1"); //fail
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2"); //fail

    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-03%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=1970-01-04%2000:00:00");

    stub_timer::value = 1 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2");
    
    stub_timer::value = 2 * 24 * 3600;
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=1");
    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=2"); //fail

    ss = curl.get("https://127.0.0.1:8181/get_statistic");
    EXPECT_EQ(ss.str(), "duration  = 1440.000000\nfrequency = 1.500000\n1970-01-01 03:00:00 2.000000\n1970-01-02 03:00:00 1.000000\n");
}



TEST(with_stub_dao, errors)
{
    server_starter <https_server_impl <stub_db, stub_timer> > server("8181");
    curl_ignore curl;

    std::stringstream ss;
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=1&client_name=wa28&use_to=1970-01-01%2000:00:00");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=2&use_to=1970-01-02%2000:00:00"); //id
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&ue_to=1970-01-02%2000:00:00"); //use_to
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonem_id=1&use_to=1970-01-03%2000:00:00"); //id
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=kjgf&use_to=1970-01-03%2000:00:00"); //id
    EXPECT_FALSE(ss.str() == "ok");
    //invalid test: stub_db does not check data format
    //ss = curl.get("https://127.0.0.1:8181/continue_abonement?abonement_id=1&use_to=asdasd"); //use_to
    //EXPECT_FALSE(ss.str() == "ok");

    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=2&clame=wa51&use_to=1970-01-02%2000:00:00"); //client
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/create_abonement?abonement_id=1&client_name=wa324&use_to=1970-01-01%2000:00:00"); //exist
    EXPECT_FALSE(ss.str() == "ok");

    ss = curl.get("https://127.0.0.1:8181/enter?abonement_id=3");
    EXPECT_FALSE(ss.str() == "ok");
    ss = curl.get("https://127.0.0.1:8181/enter?aboned=2");
    EXPECT_FALSE(ss.str() == "ok");

    ss = curl.get("https://127.0.0.1:8181/method_not_exist");
    EXPECT_FALSE(ss.str() == "ok");
}




