#include <iostream>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>

#include "database_manager.h"

using namespace std;

namespace work_with_datbase {

DBManager::DBManager(const std::string& host, const std::string& user, const std::string& pass, const std::string& db_name):
    host_(host),
    user_(user),
    pass_(pass),
    db_name_(db_name)
{
    try
    {
        sql::Driver * driver = sql::mysql::get_driver_instance();
        /* Using the Driver to create a connection */
        con_ = unique_ptr<sql::Connection>(driver->connect(host_, user_, pass_));
        con_->setSchema(db_name_);

        stmt_ = unique_ptr<sql::Statement>(con_->createStatement());
        std::unique_ptr< sql::ResultSet > res(stmt_->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));
        cout << "\t... running 'SELECT 'Welcome to Connector/C++' AS _message'" << endl;
        while (res->next()) {
          cout << "\t... MySQL replies: " << res->getString("_message") << endl;
          cout << "\t... say it again, MySQL" << endl;
          cout << "\t....MySQL replies: " << res->getString(1) << endl;
        }

        CreateDatabase();

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
    stmt_->execute("CREATE DATABASE IF NOT EXISTS poke_bat");
    stmt_->execute("USE poke_bat");
    stmt_->execute("CREATE TABLE pokemons (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, name VARCHAR(30),\
        type ENUM('NORMAL', 'FIRE','WATER','GRASS','ELECTRIC','ICE','FIGHTING','POISON','GROUND','FLYING','PSYCHIC','BUG','ROCK','GHOST','DARK','DRAGON','STEEL','FAIRY'),\
            HP BIGINT(1),\
            attack BIGINT(1),\
            defense BIGINT(1),\
            spell_attack BIGINT(1),\
            spell_defense BIGINT(1),\
            speed BIGINT(1),\
            EXP BIGINT(1),\
            LVL BIGINT(1),\
            skill ENUM('ATTACK', 'BUFF', 'DEBUFF'));"
            );

}

void DBManager::AddPokemon(Pokemon& given_pok)
{

}

Pokemon DBManager::GetPokemon(size_t level)
{

}

void DBManager::RemovePokemon()
{

}

}
