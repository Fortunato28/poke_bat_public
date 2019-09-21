#pragma once

#include <map>
#include "fight.h"
#include "gen-cpp/PokServer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using PokServerIf = poke_bat::middleware::PokServerIf;
using Config = poke_bat::middleware::Config;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;
using FightData = poke_bat::middleware::FightData;

class PokServerHandler : virtual public PokServerIf
{
 public:
  PokServerHandler();

  uint64_t last_fight_id;
  std::map<uint64_t, Fight> fight_storage;

  void getConfig(Config& _return);

  void startFight(FightData& _return, const int64_t complexity, const Pokemon& clientPokemon);

  void punch(RoundResult& _return, const int64_t fight_id);

  void defend(RoundResult& _return, const int64_t fight);

  void useSkill(RoundResult& _return, const int64_t fight, const std::string& skillName);
};
