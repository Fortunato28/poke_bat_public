#pragma once

#include <cstddef>

using lvl = std::size_t;

namespace work_with_datbase {

class Pokemon{};

class DBManager
{
public:
    DBManager();
    ~DBManager();
    void CreateDatabase();

    void AddPokemon(Pokemon& given_pok);

    Pokemon GetPokemon(lvl& level);
    void RemovePokemon();
private:
};
} // namespace
