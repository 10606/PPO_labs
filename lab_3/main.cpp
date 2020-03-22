
#include "processing/processing.h"
#include "dao/postgres.h"
#include "server/https_server.h"
#include "server/https_server_impl.h"
#include "utils/data.h"

int main ()
{
    server_starter <https_server_impl <DAO> > server("8080");
    
    int a;
    std::cin >> a;
}
