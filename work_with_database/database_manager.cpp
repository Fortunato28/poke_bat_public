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
        testPok = GetPokemonToFight("not implemented");
        // TODO delete
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
    if(con_)
    {
        delete con_;
    }
    if(stmt_)
    {
        delete stmt_;
    }
}

// Выровнять конкретный столбец таблицы
void level_off_column(vector<string>& table, const vector<size_t>& realColumnsLens, size_t& longestColumn, size_t columnStart)
{
    for(size_t i = 0; i < table.size(); ++i)
    {
        // Вычисляем пробельчики
        auto& row = table[i];
        auto amountOfSpaces = longestColumn - realColumnsLens[i];
        auto beginAmountOfSpaces = amountOfSpaces / 2;
        string beginAddedSpaces(beginAmountOfSpaces, ' ');
        string endAddedSpaces(amountOfSpaces - beginAmountOfSpaces, ' ');

        // Ниндзя-нарезка результирующей строки
        row =  row.substr(0, columnStart) +
               row.substr(columnStart, 1) +
               " " +
               beginAddedSpaces +
               row.substr(columnStart + 1, realColumnsLens[i] - 1) +
               endAddedSpaces +
               " " +
               row.substr(columnStart + realColumnsLens[i]);
    }
    longestColumn += 2; // Два дополнительных пробела
}

// Выравнивание таблицы. Рекомендую ничего не трогать, а то поедет и чекер загрустит.
void level_off_table(vector<string>& table)
{
    size_t columnStart = 0;
    size_t prevColumnWidth = 0;
    vector<size_t> realColumnsLens;
    auto columnsAmount = 9;
    for(size_t i = 0; i < columnsAmount; ++i)
    {
        size_t longestColumn = 0;
        for(auto& row: table)
        {
            auto absCurrentColumnLen = row.find('|', columnStart + 1);
            auto relativeCurrentColumnLen = absCurrentColumnLen - columnStart;

            if(relativeCurrentColumnLen > longestColumn)
            {
                longestColumn = relativeCurrentColumnLen;
            }

            realColumnsLens.push_back(relativeCurrentColumnLen);
        }

        level_off_column(table, realColumnsLens, longestColumn, columnStart);

        // Зададим начало следующего столбца
        columnStart += longestColumn;
        prevColumnWidth = longestColumn;
        realColumnsLens.clear();
    }
}

const std::string DBManager::GetSavedPoks()
{
    string query = "SELECT pub_id, name, type, HP, attack, defense, spell_attack, spell_defense, LVL FROM " + table_name + ";";

    sql::ResultSet* res(stmt_->executeQuery(query));

    vector<string> table;
    table.push_back("|public_id|name|type|HP|attack|defense|spell_attack|spell_defense|LVL|\n");
    while(res->next())
    {
        string row;
        row += "|";
        row += res->getString("pub_id");
        row += "|";
        row += res->getString("name");
        row += "|";
        row += res->getString("type");
        row += "|";
        row += res->getString("HP");
        row += "|";
        row += res->getString("attack");
        row += "|";
        row += res->getString("defense");
        row += "|";
        row += res->getString("spell_attack");
        row += "|";
        row += res->getString("spell_defense");
        row += "|";
        row += res->getString("LVL");
        row += "|\n";

        table.push_back(row);
    }

    level_off_table(table);

    // Наполнение результирующей строки
    string separatorRow(table[0].length() - 1, '-');
    separatorRow += "\n";
    table.insert(std::next(table.begin()), separatorRow);
    string result;
    result += separatorRow;
    for(auto& row: table)
    {
        result += row;
    }
    result += separatorRow;

    delete res;
    return result;
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

const std::string DBManager::GetComment(const string& pub_id)
{
    const string query = "SELECT comment FROM saved_pokemons WHERE pub_id='" + pub_id + "';";
    unique_ptr<sql::ResultSet> res(stmt_->executeQuery(query));
    if(res->next())
    {
        auto result = res->getString("comment");
        return result;
    }
    return {};
}

Pokemon DBManager::GetPokemonToFight(const string& pub_id)
{
    const string getPokemonCommand = "SELECT * FROM " + table_name + " WHERE pub_id=" + "'" + pub_id + "'" + ";";
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
            gottenPokemon.__set_pub_id(res->getString("pub_id"));
        }
        // Нет такого покемона в базе
        else
        {
            gottenPokemon.__set_EXP(-1);
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
