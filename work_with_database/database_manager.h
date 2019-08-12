#pragma once

#include <memory>
#include <cstddef>

#include <cppconn/statement.h>

// TODO сделай нормально
#include "../middleware//gen-cpp/interfaces_types.h"

namespace work_with_datbase {

// TODO Think about it, mb it's bad
#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "1"
#define EXAMPLE_DB "poke_bat"

// TODO Пофиксить на нормальный класс покемонов
using Pokemon = poke_bat::middleware::Pokemon;
//class Pokemon{};

class DBManager
{
public:
    DBManager(const std::string& host, const std::string& user, const std::string& pass, const std::string& db_name);
    ~DBManager();

    void AddPokemon(Pokemon& given_pok);

    Pokemon GetPokemon(size_t complexity);
    void UpdatePokemon(std::string name, size_t complexity);
    void RemovePokemon();
private:

    // Тестовый метод, по факту при работе сервиса ничего создаваться не должно
    void CreateDatabase();

    // Данные по базе
    const std::string host_;
    const std::string user_;
    const std::string pass_;
    const std::string db_name_;

    // Держалки коннекта к базе
    std::unique_ptr<sql::Statement> stmt_;
    std::unique_ptr<sql::Connection> con_;
};
} // namespace
