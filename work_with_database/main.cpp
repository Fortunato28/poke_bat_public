#include <iostream>

#include "database_manager.h"

using namespace std;
using namespace work_with_datbase;

int main()
{
    DBManager db_manager("localhost", "root", "1", "poke_bat");

    cout << "Worked minimal!" << endl;
}
