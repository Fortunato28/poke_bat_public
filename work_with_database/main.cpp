#include <iostream>

#include "database_manager.h"

using namespace std;
using namespace work_with_datbase;

// TODO Think about it, mb it's bad
#define host "172.17.0.2"
const std::string user = "root";
const std::string pass = "1";
const std::string db = "poke_bat";

int main()
{
    DBManager db_manager(host, user, pass, db);

    printf("HERE %s\n", "WORKED MINIMAL");
}
