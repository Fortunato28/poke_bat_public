#include <iostream>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "database_manager.h"

using namespace std;

namespace work_with_datbase {

DBManager::DBManager()
{
    string url(EXAMPLE_HOST);
    const string user(EXAMPLE_USER);
    const string pass( EXAMPLE_PASS);
    const string database(EXAMPLE_DB);

    try
    {
        sql::Driver * driver = sql::mysql::get_driver_instance();
        /* Using the Driver to create a connection */
        std::unique_ptr< sql::Connection > con(driver->connect(url, user, pass));
        con->setSchema(database);

        std::unique_ptr< sql::Statement > stmt(con->createStatement());
        std::unique_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));
        cout << "\t... running 'SELECT 'Welcome to Connector/C++' AS _message'" << endl;
        while (res->next()) {
          cout << "\t... MySQL replies: " << res->getString("_message") << endl;
          cout << "\t... say it again, MySQL" << endl;
          cout << "\t....MySQL replies: " << res->getString(1) << endl;
        }

    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

}

DBManager::~DBManager()
{

}

void DBManager::CreateDatabase()
{

}

void DBManager::AddPokemon(Pokemon& given_pok)
{

}

Pokemon DBManager::GetPokemon(lvl& level)
{

}

void DBManager::RemovePokemon()
{

}

}
