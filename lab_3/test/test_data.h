#include <gtest/gtest.h>

#include "../utils/data.h"

TEST(data, abonement)
{
    abonement a(1, "a", "b");
    EXPECT_EQ(a.id, 1);
    EXPECT_EQ(a.client_name, "a");
    EXPECT_EQ(a.use_to, "b");
    EXPECT_EQ(to_string(a), "1 a -> b"); 

    std::map <std::string, std::string> args = 
    {
        {"use_to", "q"},
        {"client_name", "w"},
        {"unused_param", ""},
        {"abonement_id", "213"}
    };
    abonement b(args);
    EXPECT_EQ(a, a);
    EXPECT_FALSE(a == b);
    EXPECT_EQ(b.id, 213);
    EXPECT_EQ(b.client_name, "w");
    EXPECT_EQ(b.use_to, "q");
    EXPECT_EQ(to_string(b), "213 w -> q"); 
}

TEST(data, abonement_continue)
{
    abonement_continue a(2, "z");
    EXPECT_EQ(a.id, 2);
    EXPECT_EQ(a.use_to, "z");
    
    std::map <std::string, std::string> args =
    {
        {"use_to", "f"},
        {"abonement_id", "123"},
        {"unused", "trefsfv  redf "}
    };
    abonement_continue b(args);
    EXPECT_EQ(a, a);
    EXPECT_FALSE(a == b);
    EXPECT_EQ(b.use_to, "f");
    EXPECT_EQ(b.id, 123);
}

TEST(data, enter)
{
    enter e(1, "qwe");
    EXPECT_EQ(e.id, 1);
    EXPECT_EQ(e.time, "qwe");

    EXPECT_EQ(e, e);
    EXPECT_FALSE(e == enter(2, "qwe"));
}

