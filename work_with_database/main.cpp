#include <iostream>

#include "database_manager.h"

using namespace std;
using namespace work_with_datbase;

int main()
{
    DBManager db_manager(EXAMPLE_HOST, EXAMPLE_USER, EXAMPLE_PASS, EXAMPLE_DB);

    cout << "Worked minimal!" << endl;
}
