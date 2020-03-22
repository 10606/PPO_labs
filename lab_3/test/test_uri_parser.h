#include <gtest/gtest.h>

#include "../server/uri_parser.h"

TEST(uri_parser, base)
{
    uri parsed = parse_uri ("fasdsf/jyhgtr/nhbgf?dsf=bgvf&yjnhtbgrvf=Jnhytbg&mjnhbg=23.e4");

    EXPECT_EQ(parsed.path, "fasdsf/jyhgtr/nhbgf");
    EXPECT_EQ(parsed.args.size(), 3);
    EXPECT_EQ(parsed.args[0].key, "dsf");
    EXPECT_EQ(parsed.args[0].value, "bgvf");
    EXPECT_EQ(parsed.args[1].key, "yjnhtbgrvf");
    EXPECT_EQ(parsed.args[1].value, "Jnhytbg");
    EXPECT_EQ(parsed.args[2].key, "mjnhbg");
    EXPECT_EQ(parsed.args[2].value, "23.e4");
}

TEST(uri_parser, empty_path)
{
    uri parsed = parse_uri ("?dsf=bgvf&yjnhtbgrvf=Jnhytbg&mjnhbg=23.e4");

    EXPECT_EQ(parsed.path, "");
    EXPECT_EQ(parsed.args.size(), 3);
    EXPECT_EQ(parsed.args[0].key, "dsf");
    EXPECT_EQ(parsed.args[0].value, "bgvf");
    EXPECT_EQ(parsed.args[1].key, "yjnhtbgrvf");
    EXPECT_EQ(parsed.args[1].value, "Jnhytbg");
    EXPECT_EQ(parsed.args[2].key, "mjnhbg");
    EXPECT_EQ(parsed.args[2].value, "23.e4");
}

TEST(uri_parser, empty_args_1)
{
    uri parsed = parse_uri ("fasdsf/jyhgtr/nhbgf?");

    EXPECT_EQ(parsed.path, "fasdsf/jyhgtr/nhbgf");
    EXPECT_EQ(parsed.args.size(), 0);
}

TEST(uri_parser, empty_args_2)
{
    uri parsed = parse_uri ("fasdsf/jyhgtr/nhbgf");

    EXPECT_EQ(parsed.path, "fasdsf/jyhgtr/nhbgf");
    EXPECT_EQ(parsed.args.size(), 0);
}


TEST(uri_parser, empty_key)
{
    uri parsed = parse_uri ("fasdsf/jyhgtr/nhbgf?dsf=bgvf&yjnhtbgrvf=Jnhytbg&=23.e4");

    EXPECT_EQ(parsed.path, "fasdsf/jyhgtr/nhbgf");
    EXPECT_EQ(parsed.args.size(), 3);
    EXPECT_EQ(parsed.args[0].key, "dsf");
    EXPECT_EQ(parsed.args[0].value, "bgvf");
    EXPECT_EQ(parsed.args[1].key, "yjnhtbgrvf");
    EXPECT_EQ(parsed.args[1].value, "Jnhytbg");
    EXPECT_EQ(parsed.args[2].key, "");
    EXPECT_EQ(parsed.args[2].value, "23.e4");
}


TEST(uri_parser, empty_value)
{
    uri parsed = parse_uri ("fasdsf/jyhgtr/nhbgf?dsf=bgvf&yjnhtbgrvf=&mjnhbg=23.e4");

    EXPECT_EQ(parsed.path, "fasdsf/jyhgtr/nhbgf");
    EXPECT_EQ(parsed.args.size(), 3);
    EXPECT_EQ(parsed.args[0].key, "dsf");
    EXPECT_EQ(parsed.args[0].value, "bgvf");
    EXPECT_EQ(parsed.args[1].key, "yjnhtbgrvf");
    EXPECT_EQ(parsed.args[1].value, "");
    EXPECT_EQ(parsed.args[2].key, "mjnhbg");
    EXPECT_EQ(parsed.args[2].value, "23.e4");
}

