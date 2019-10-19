#include <iostream>

#include "database_manager.h"
#include "db_conf.h"

using namespace std;
using namespace work_with_datbase;

int main()
{
    DBManager db_manager(host, 3306, user, pass, db);

    printf("HERE %s\n", "WORKED MINIMAL");
}
