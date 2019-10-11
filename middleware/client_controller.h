#pragma once
#include <memory>

#include "gen-cpp/PokServer.h"

using PokServerIf = poke_bat::middleware::PokServerIf;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;
using FightData = poke_bat::middleware::FightData;

class ClientController
{
public:
    ClientController();
    ~ClientController();

    void getConfig(std::string& _return);

    void startFight(FightData& _return, const int64_t complexity, const Pokemon& clientPokemon);

    void punch(RoundResult& _return);

    void defend(RoundResult& _return);

    void useSkill(RoundResult& _return, const std::string& skillName);

    std::string savePokemon(const std::string& private_id, const Pokemon& c_pok, const std::string& comment);

    const std::string getSavedPoksTable();

private:
    // Простите, но это синглтон
    ClientController(const ClientController& other) = delete;
    ClientController(ClientController&& other) = delete;
    ClientController& operator=(const ClientController& other) = delete;
    ClientController& operator=(ClientController&& other) = delete;

    std::unique_ptr<poke_bat::middleware::PokServerClient> thrift_client_;
    std::shared_ptr<apache::thrift::protocol::TTransport> transport_;

    uint64_t fight_id_;
};
