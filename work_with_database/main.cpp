#include <iostream>

#include "database_manager.h"

using namespace std;
using namespace work_with_datbase;

int main()
{
    std::string host = "127.0.0.1";
    int port = 3306;
    std::string user = "user";
    std::string pass = "pass";
    std::string dbname = "poke_bat";

    DBManager db_manager(host, 3306, user, pass, dbname);

    printf("HERE %s\n", "WORKED MINIMAL");
}
