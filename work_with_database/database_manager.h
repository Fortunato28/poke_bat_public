#pragma once

#include <cstddef>

using lvl = std::size_t;

namespace work_with_datbase {

// TODO Think about it, mb it's bad
#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "1"
#define EXAMPLE_DB "test"

class Pokemon{};

class DBManager
{
public:
    DBManager();
    ~DBManager();

    void AddPokemon(Pokemon& given_pok);

    Pokemon GetPokemon(lvl& level);
    void RemovePokemon();
private:

    // Тестовый метод, по факту при работе сервиса ничего создаваться не должно
    void CreateDatabase();
};
} // namespace
