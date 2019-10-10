#include <iostream>
#include <map>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>

#include "database_manager.h"
#include "utilities.h"
#include "gen-cpp/interfaces_types.h"

using namespace std;
using namespace poke_bat::middleware;

namespace work_with_datbase {

DBManager::DBManager(const std::string host, const std::string user, const std::string pass, const std::string db_name):
    host_(host),
    user_(user),
    pass_(pass),
    db_name_(db_name),
    stmt_(nullptr),
    con_(nullptr)
{
    try
    {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();

        ///* Using the Driver to create a connection */
        con_ = driver->connect(host_, user_, pass_);
        if(con_->isValid())
        {
            printf("Connected to database succesfully\n");
        }
        else throw::runtime_error("Cannot connect to database!");
        con_->setSchema(db_name_);

        stmt_ = con_->createStatement();

        CreateTable();

        // TODO Это залипуха только для тестирования
        Pokemon testPok;
        testPok = GetPokemon(3);
        cout << testPok.skill << endl;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        throw std::runtime_error("Cannot connect to database!");
    }
    catch (std::exception &e)
    {
        printf("HERE %s\n", e.what());
    }
}

DBManager::~DBManager()
{
    // TODO is it safe?
    if(con_)
    {
        delete con_;
    }
    if(stmt_)
    {
        delete stmt_;
    }
}

void DBManager::CreateTable()
{
    stmt_->execute("CREATE DATABASE IF NOT EXISTS poke_bat;");
    stmt_->execute("USE poke_bat;");
    stmt_->execute("CREATE TABLE IF NOT EXISTS " + table_name + "\
        (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,\
        name VARCHAR(30),\
        type ENUM(\
                'NORMAL',\
                'FIRE',\
                'WATER',\
                'GRASS',\
                'ELECTRIC',\
                'ICE',\
                'FIGHTING',\
                'POISON',\
                'GROUND',\
                'FLYING',\
                'PSYCHIC',\
                'BUG',\
                'ROCK',\
                'GHOST',\
                'DARK',\
                'DRAGON',\
                'STEEL',\
                'FAIRY'\
            ),\
            HP BIGINT(1),\
            attack BIGINT(1),\
            defense BIGINT(1),\
            spell_attack BIGINT(1),\
            spell_defense BIGINT(1),\
            speed BIGINT(1),\
            EXP BIGINT(1),\
            LVL BIGINT(1),\
            skill LONGTEXT,\
            comment LONGTEXT,\
            private_id LONGTEXT,\
            pub_id LONGTEXT);"
            );
}

void DBManager::SavePokemon(const std::string& private_id,
                            const std::string& pub_id,
                            const Pokemon& pok,
                            const std::string& comment)
{
    // Sorry about this shit
    string query = "INSERT INTO " +
            table_name +
            " VALUES (NULL, " +
            "'" + pok.name + "', " +
            "'" + utilities::get_string_poketype(pok.type) + "', " +
            "'" + to_string(pok.HP) + "', " +
            "'" + to_string(pok.attack) + "', " +
            "'" + to_string(pok.defense) + "', " +
            "'" + to_string(pok.spell_attack) + "', " +
            "'" + to_string(pok.spell_defense) + "', " +
            "'" + to_string(pok.speed) + "', " +
            "'" + to_string(pok.EXP) + "', " +
            "'" + to_string(pok.LVL) + "', " +
            "'" + pok.skill.name + "_" + utilities::get_string_pokeskilltype(pok.skill.type) + "_" + to_string(pok.skill.amount) + "', " +
            "'" + comment + "', " +
            "'" + private_id + "', " +
            "'" + pub_id + "');";

    stmt_->execute(query);
}

// TODO если костыль сработает, то вынести отдельно
PokemonSkill parseStringFromDB(const std::string& str)
{
    string skillData = str;
    auto skillName = skillData.substr(0, skillData.find('_'));
    skillData.erase(0, skillName.length() + 1);

    auto skillType = skillData.substr(0, skillData.find('_'));
    skillData.erase(0, skillType.length() + 1);

    auto skillAmount = skillData;

    PokemonSkill pokemonSkill;
    pokemonSkill.__set_name(skillName);
    pokemonSkill.__set_type(utilities::get_enum_pokeskilltype(skillType));
    pokemonSkill.__set_amount(atoi(skillAmount.c_str()));
    return pokemonSkill;
}

Pokemon DBManager::GetPokemon(size_t level)
{
    // FIXME Правильно ли я понимаю, что тут возможно инъекция?
    string getPokemonCommand = "SELECT * FROM " + table_name + " WHERE LVL=" + to_string(level) + ";";
        sql::ResultSet* res(stmt_->executeQuery(getPokemonCommand));

        Pokemon gottenPokemon;
        if(res->next())
        {
            gottenPokemon.__set_name(res->getString(2));
            gottenPokemon.__set_type(utilities::get_enum_poketype(res->getString(3)));
            gottenPokemon.__set_HP(res->getInt(4));
            gottenPokemon.__set_attack(res->getInt(5));
            gottenPokemon.__set_defense(res->getInt(6));
            gottenPokemon.__set_spell_attack(res->getInt(7));
            gottenPokemon.__set_spell_defense(res->getInt(8));
            gottenPokemon.__set_speed(res->getInt(9));
            gottenPokemon.__set_EXP(res->getInt(10));
            gottenPokemon.__set_LVL(res->getInt(11));
            gottenPokemon.__set_skill(parseStringFromDB(res->getString(12)));
            // TODO что-то странное, подумать-передумать
            gottenPokemon.__set_flag(res->getString(13));
            // TODO скорее всего, должны быть в другом методе
            printf("HERE %s\n", res->getString(14).c_str());
            printf("HERE %s\n", res->getString(15).c_str());
        }

        if(res)
        {
            delete res;
        }
        return gottenPokemon;
}

void DBManager::RemovePokemon()
{
    string deleteCommand = "DELETE FROM " + table_name + " WHERE spell_attack=5;";
    stmt_->execute(deleteCommand);
}

}
