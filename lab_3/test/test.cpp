#include "test_data.h"
#include "test_uri_parser.h"
#include "test_processing.h"
#include "test_with_stub_dao.h"

int main (int argc, char * argv [])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

