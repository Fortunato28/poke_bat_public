#pragma once
#include <memory>

#include "gen-cpp/PokServer.h"

using PokServerIf = poke_bat::middleware::PokServerIf;
using Config = poke_bat::middleware::Config;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;

class ClientController
{
public:
    ClientController();
    ~ClientController();

    void getConfig(Config& _return);

    void startFight(Pokemon& _return, const int64_t complexity, const Pokemon& clientPokemon);

    void punch(RoundResult& _return);

    void defend(RoundResult& _return);

    void useSkill(RoundResult& _return, const std::string& skillName);

private:
    // Простите, но это синглтон
    ClientController(const ClientController& other) = delete;
    ClientController(ClientController&& other) = delete;
    ClientController& operator=(const ClientController& other) = delete;
    ClientController& operator=(ClientController&& other) = delete;

    std::unique_ptr<poke_bat::middleware::PokServerClient> thrift_client_;
    std::shared_ptr<apache::thrift::protocol::TTransport> transport_;
};
