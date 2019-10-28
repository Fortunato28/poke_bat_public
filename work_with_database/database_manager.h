#pragma once

#include <memory>
#include <cstddef>
#include <memory>
#include <mutex>

#include <cppconn/statement.h>

#include "gen-cpp/interfaces_types.h"

namespace work_with_datbase {

using Pokemon = poke_bat::middleware::Pokemon;

class DBManager
{
public:
    DBManager(const std::string host, const int, const std::string user, const std::string pass, const std::string db_name);
    ~DBManager();

    void ConnectToDB();
    void ReconnectToDB();
    std::string SavePokemon(const std::string& private_id,
                     const std::string& pub_id,
                     const Pokemon& given_pok,
                     const std::string& comment);

    Pokemon GetPokemonToFight(const std::string& pub_id);
    void UpdatePokemon(std::string& name, size_t complexity);
    void RemovePokemon();
    const std::string GetSavedPoks();
    const std::string GetComment(const std::string& pub_id);
    Pokemon GetPokByPrivateID(const std::string& private_id);

private:
    void CreateTable();

    // Данные по базе
    const std::string host_;
    const int port_;
    const std::string user_;
    const std::string pass_;
    const std::string db_name_;

    // Держалки коннекта к базе
    std::unique_ptr<sql::Statement> stmt_;
    std::unique_ptr<sql::Connection> con_;

    const std::string table_name = "saved_pokemons";

    // Мьютекс для синхронизации доступа к драйверу
    std::mutex driver_security;
};
} // namespace
