#pragma once

#include <map>
#include "fight.h"
#include "gen-cpp/PokServer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "database_manager.h"

using PokServerIf = poke_bat::middleware::PokServerIf;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;
using FightData = poke_bat::middleware::FightData;

class PokServerHandler : virtual public PokServerIf
{
 public:
  PokServerHandler();

  void getConfig(std::string& _return);

  void startFight(FightData& _return, const int64_t complexity, const Pokemon& clientPokemon);

  void punch(RoundResult& _return, const int64_t fight_id);

  void defend(RoundResult& _return, const int64_t fight);

  void useSkill(RoundResult& _return, const int64_t fight, const std::string& skillName);

  Fight& findFight(const int64_t &fight_id);

  void savePokemon(std::string& _return, const std::string& private_id, const Pokemon& client_pokemon, const std::string& comment);

  void getSavedPoksTable(std::string& _return);

private:
  bool isFightStopped(RoundResult& roundResult_, const Pokemon& server, const Pokemon& client, const int64_t fight_id);

  uint64_t next_fight_id_;
  std::map<uint64_t, Fight> fight_storage_;
  work_with_datbase::DBManager dbManager_;
};
