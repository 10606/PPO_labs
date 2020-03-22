#include <atomic>
#include <gtest/gtest.h>

#include "curl_wrap.h"

struct curl_ignore : public curl_wrap
{
    virtual std::stringstream get (std::string const & request, std::map <std::string, std::string> const & properties = {})
    {
        set_opt(CURLOPT_SSL_VERIFYPEER, 0L);
        return curl_wrap::get(request, properties);
    }
};

        
TEST(base, store_users)
{
    curl_ignore curl;
    
    curl.get("127.0.0.1:8080/clear");
    curl.get("127.0.0.1:8080/addUser?user_name=a&user_login=b&user_currency=dollar&user_id=1");
    curl.get("127.0.0.1:8080/addUser?user_name=c&user_login=d&user_currency=euro&user_id=2");
    try
    {
        curl.get("127.0.0.1:8080/addUser?no_user_name=c&user_login=d&user_currency=euro&user_id=2");
        curl.get("127.0.0.1:8080/addUser?user_name=c&no_user_login=d&user_currency=euro&user_id=2");
        curl.get("127.0.0.1:8080/addUser?user_name=c&user_login=d&no_user_currency=euro&user_id=2");
        curl.get("127.0.0.1:8080/addUser?user_name=c&user_login=d&user_currency=euro&no_user_id=2");
        
        curl.get("127.0.0.1:8080/addUser?user_name=q&user_login=w&user_currency=euro&user_id=qwe");
        curl.get("127.0.0.1:8080/addUser?user_name=q&user_login=w&user_currency=ewqeqw&user_id=2");
    }
    catch (...)
    {}
    curl.get("127.0.0.1:8080/addUser?user_name=e&user_login=f&user_currency=rub&user_id=3");

    std::stringstream ans = curl.get("127.0.0.1:8080/users");
    std::string etalon = 
        "User{id=1, name='a', login='b', currency='dollar'}\n"
        "User{id=2, name='c', login='d', currency='euro'}\n"
        "User{id=3, name='e', login='f', currency='rub'}\n";
    EXPECT_EQ(ans.str(), etalon);
}


struct object
{
    size_t id;
    std::string name;
    std::string cnt;
    double cost;
};

std::istream & operator >> (std::istream & s, object & value)
{
    s.ignore(std::numeric_limits<std::streamsize>::max(), '=');
    s >> value.id;
    s.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
    std::getline(s, value.name, '\'');
    s.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
    std::getline(s, value.cnt, '\'');
    s.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
    s >> value.cost;
    s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return s;
}

std::ostream & operator << (std::ostream & s, object const & value)
{
    s << "id = " << value.id << "  name = " << value.name << "  cnt = " << value.cnt << "  cost = " << value.cost;
    return s;
}

void read_objects_to_end (std::vector <object> & obj1, std::istream & o1)
{
    while (!o1.eof())
    {
        object u;
        o1 >> u;
        if (o1.eof())
        {
            break;
        }
        obj1.push_back(u);
        //std::cout << u << "\n";
    }
}

TEST(base, add_object)
{
    curl_ignore curl;
    std::stringstream c = curl.get("http://127.0.0.1:8080/currency");
    std::map <std::string, double> currency;
    while (c && !c.eof() && !c.bad() && !c.fail())
    {
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        if (c.eof())
        {
            break;
        }
        std::string type;
        std::getline(c, type, '\'');
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        double value;
        c >> value;
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        currency.insert({type, value});
    }

    curl.get("127.0.0.1:8080/clear");

    curl.get("http://127.0.0.1:8080/addUser?user_name=a&user_login=b&user_currency=dollar&user_id=1");
    curl.get("http://127.0.0.1:8080/addUser?user_name=c&user_login=d&user_currency=euro&user_id=2");
    curl.get("http://127.0.0.1:8080/addUser?user_name=e&user_login=f&user_currency=rub&user_id=3");

    curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=1&object_name=a&object_cost=10&object_cnt=2");
    curl.get("http://127.0.0.1:8080/addObject?user_id=2&object_id=2&object_name=b&object_cost=5&object_cnt=3");
    try
    {
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=1&object_name=a&object_cost=11&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=4&object_id=5&object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?no_user_id=1&object_id=5&object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&no_object_id=5&object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=5&no_object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=5&object_name=a&no_object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=5&object_name=a&object_cost=10&no_object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=234234&object_id=5&object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=rwerww&object_name=a&object_cost=10&object_cnt=2");
        curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=1&object_name=a&object_cost=werrwer&object_cnt=2");
    }
    catch (...)
    {}
    curl.get("http://127.0.0.1:8080/addObject?user_id=3&object_id=3&object_name=c&object_cost=15&object_cnt=4");

    std::stringstream o1 = curl.get("http://127.0.0.1:8080/getObject?user_id=1");
    std::stringstream o2 = curl.get("http://127.0.0.1:8080/getObject?user_id=2");
    std::stringstream o3 = curl.get("http://127.0.0.1:8080/getObject?user_id=3");

    
    std::vector <object> obj1;
    read_objects_to_end(obj1, o1);
    EXPECT_EQ(obj1.size(), 3);

    std::vector <object> obj2;
    read_objects_to_end(obj2, o2);
    EXPECT_EQ(obj2.size(), 3);

    std::vector <object> obj3;
    read_objects_to_end(obj3, o3);
    EXPECT_EQ(obj3.size(), 3);

    double etalon;
    
    etalon = 10 * currency["dollar"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[0].cost, etalon);
    etalon = 5  * currency["euro"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[1].cost, etalon);
    etalon = 15 * currency["rub"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[2].cost, etalon);

    etalon = 10 * currency["dollar"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[0].cost, etalon);
    etalon = 5  * currency["euro"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[1].cost, etalon);
    etalon = 15 * currency["rub"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[2].cost, etalon);

    etalon = 10 * currency["dollar"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[0].cost, etalon);
    etalon = 5  * currency["euro"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[1].cost, etalon);
    etalon = 15 * currency["rub"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[2].cost, etalon);
}


TEST(base, add_object_)
{
    curl_ignore curl;
    std::stringstream c = curl.get("http://127.0.0.1:8080/currency");
    std::map <std::string, double> currency;
    while (c && !c.eof() && !c.bad() && !c.fail())
    {
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        if (c.eof())
        {
            break;
        }
        std::string type;
        std::getline(c, type, '\'');
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        double value;
        c >> value;
        c.ignore(std::numeric_limits<std::streamsize>::max(), '\'');
        currency.insert({type, value});
    }

    curl.get("127.0.0.1:8080/clear");

    curl.get("http://127.0.0.1:8080/addUser?user_name=a&user_login=b&user_currency=dollar&user_id=3");
    curl.get("http://127.0.0.1:8080/addUser?user_name=c&user_login=d&user_currency=euro&user_id=2");
    curl.get("http://127.0.0.1:8080/addUser?user_name=e&user_login=f&user_currency=rub&user_id=1");

    curl.get("http://127.0.0.1:8080/addObject?user_id=3&object_id=1&object_name=a&object_cost=20&object_cnt=2");
    curl.get("http://127.0.0.1:8080/addObject?user_id=2&object_id=2&object_name=b&object_cost=15&object_cnt=3");
    curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=3&object_name=c&object_cost=25&object_cnt=4");
    curl.get("http://127.0.0.1:8080/addObject?user_id=1&object_id=4&object_name=c&object_cost=35&object_cnt=4");

    std::stringstream o1 = curl.get("http://127.0.0.1:8080/getObject?user_id=3");
    std::stringstream o2 = curl.get("http://127.0.0.1:8080/getObject?user_id=2");
    std::stringstream o3 = curl.get("http://127.0.0.1:8080/getObject?user_id=1");

    
    std::vector <object> obj1;
    read_objects_to_end(obj1, o1);
    EXPECT_EQ(obj1.size(), 4);

    std::vector <object> obj2;
    read_objects_to_end(obj2, o2);
    EXPECT_EQ(obj2.size(), 4);

    std::vector <object> obj3;
    read_objects_to_end(obj3, o3);
    EXPECT_EQ(obj3.size(), 4);

    double etalon;
    
    etalon = 20 * currency["dollar"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[0].cost, etalon);
    etalon = 15  * currency["euro"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[1].cost, etalon);
    etalon = 25 * currency["rub"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[2].cost, etalon);
    etalon = 35 * currency["rub"] / currency["dollar"];
    EXPECT_DOUBLE_EQ(obj1[3].cost, etalon);

    etalon = 20 * currency["dollar"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[0].cost, etalon);
    etalon = 15  * currency["euro"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[1].cost, etalon);
    etalon = 25 * currency["rub"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[2].cost, etalon);
    etalon = 35 * currency["rub"] / currency["euro"];
    EXPECT_DOUBLE_EQ(obj2[3].cost, etalon);

    etalon = 20 * currency["dollar"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[0].cost, etalon);
    etalon = 15  * currency["euro"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[1].cost, etalon);
    etalon = 25 * currency["rub"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[2].cost, etalon);
    etalon = 35 * currency["rub"] / currency["rub"];
    EXPECT_DOUBLE_EQ(obj3[3].cost, etalon);
}






int main (int argc, char * argv [])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

