#include <iostream>
#include <map>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>

#include "database_manager.h"

using namespace std;
using namespace poke_bat::middleware;

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

        CreateDatabase();
        Pokemon test;
        AddPokemon(test);
        test = GetPokemon(1);
        // Тестово удаляет всех покемонов
        RemovePokemon();
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
    // TODO Имя pokemons вынести отдельным полем
    stmt_->execute("CREATE TABLE IF NOT EXISTS pokemons\
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
            flag LONGTEXT);"
            );

}

void DBManager::AddPokemon(Pokemon& given_pok)
{
    // TODO Добавить нарезку строки от пришедшего покемона
    stmt_->execute("INSERT INTO pokemons VALUES (NULL, 'ZHOPA', 'FIRE', 20, 20, 10, 5, 5, 15, 0, 1, 'lightning_ATTACK_20', 'isib_wtf{some_flag}');");
}

// TODO если костыль сработает, то вынести отдельно
// TODO написать парсинг строки в структуру
PokemonSkill parseStringFromDB(const std::string& skillData)
{
    return PokemonSkill();
}

Pokemon DBManager::GetPokemon(size_t level)
{
    // FIXME Правильно ли я понимаю, что тут возможно инъекция?
    string getPokemonCommand = "SELECT * FROM pokemons WHERE LVL=" + to_string(level) + ";";
        std::unique_ptr<sql::ResultSet> res(stmt_->executeQuery(getPokemonCommand));

        // TODO если костыль сработает, то вынести отдельно
        std::map<string, PokemonType::type> pokTypes
        {
            {"NORMAL", PokemonType::NORMAL},
            {"FIRE", PokemonType::FIRE},
            {"WATER", PokemonType::WATER},
            {"GRASS", PokemonType::GRASS},
            {"ELECTRIC", PokemonType::ELECTRIC},
            {"ICE", PokemonType::ICE},
            {"FIGHTING", PokemonType::FIGHTING},
            {"POISON", PokemonType::POISON},
            {"GROUND", PokemonType::GROUND},
            {"FLYING", PokemonType::FLYING},
            {"PSYCHIC", PokemonType::PSYCHIC},
            {"BUG", PokemonType::BUG},
            {"ROCK", PokemonType::ROCK},
            {"GHOST", PokemonType::GHOST},
            {"DARK", PokemonType::DARK},
            {"DRAGON", PokemonType::DRAGON},
            {"STEEL", PokemonType::STEEL},
            {"FAIRY", PokemonType::FAIRY}
        };

        std::map<string, SkillType::type> pokSkillTypes
        {
            {"ATTACK", SkillType::ATTACK},
            {"BUFF", SkillType::BUFF},
            {"DEBUFF", SkillType::DEBUFF},
        };

        Pokemon gettedPokemon;
        if(res->next())
        {
            gettedPokemon.__set_name(res->getString(2));
            gettedPokemon.__set_type(pokTypes.find(res->getString(3))->second);
            gettedPokemon.__set_HP(res->getInt(4));
            gettedPokemon.__set_attack(res->getInt(5));
            gettedPokemon.__set_defense(res->getInt(6));
            gettedPokemon.__set_spell_attack(res->getInt(7));
            gettedPokemon.__set_spell_defense(res->getInt(8));
            gettedPokemon.__set_speed(res->getInt(9));
            gettedPokemon.__set_EXP(res->getInt(10));
            gettedPokemon.__set_LVL(res->getInt(11));
            PokemonSkill gettedFromDBSkill;
            gettedPokemon.__set_skill(parseStringFromDB(res->getString(12)));

            cout << "\t... MySQL replies: " << res->getString(2) << endl;
            cout << "\t... MySQL replies: " << res->getString(3) << endl;
            cout << "\t... MySQL replies: " << res->getInt(4) << endl;
            cout << "\t... MySQL replies: " << res->getInt(5) << endl;
            cout << "\t... MySQL replies: " << res->getInt(6) << endl;
            cout << "\t... MySQL replies: " << res->getInt(7) << endl;
            cout << "\t... MySQL replies: " << res->getInt(8) << endl;
            cout << "\t... MySQL replies: " << res->getInt(9) << endl;
            cout << "\t... MySQL replies: " << res->getInt(10) << endl;
            cout << "\t... MySQL replies: " << res->getInt(11) << endl;
            cout << "\t... MySQL replies: " << res->getString(12) << endl;
            cout << "\t... MySQL replies: " << res->getString(13) << endl;
        }

        return Pokemon();
}

void DBManager::RemovePokemon()
{
    string deleteCommand = "DELETE FROM pokemons WHERE spell_attack=5;";
    cout << "ALOE BLYA" << endl;
    stmt_->execute(deleteCommand);
}

}
