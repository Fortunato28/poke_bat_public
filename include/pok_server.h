#pragma once

#include <map>
#include "fight.h"
#include "gen-cpp/PokServer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "database_manager.h"
#include "server_config_handler.h"

using PokServerIf = poke_bat::middleware::PokServerIf;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;
using FightData = poke_bat::middleware::FightData;

class PokServerHandler : virtual public PokServerIf
{
 public:
  PokServerHandler(
    const std::string &dbhost, 
    int dbport, 
    const std::string &dbuser, 
    const std::string &dbpass, 
    const std::string &dbname
  );

  void getConfig(std::string& _return);

  void startFight(FightData& _return, const std::string& pub_id, const Pokemon& clientPokemon);

  void punch(RoundResult& _return, const int64_t fight_id);

  void defend(RoundResult& _return, const int64_t fight);

  void useSkill(RoundResult& _return, const int64_t fight, const std::string& skillName);

  Fight& findFight(const int64_t &fight_id);

  void savePokemon(std::string& _return, const std::string& private_id, const Pokemon& client_pokemon, const std::string& comment);

  void getSavedPoksTable(std::string& _return);

  void getSavedPokByPrivateID(Pokemon& _return, const std::string& private_id);

private:
  bool isFightStopped(RoundResult& roundResult_, Fight& current_fight, const int64_t fight_id);

  void lvlUp(Fight& current_fight);
  bool isLvlUp(Pokemon& pok);

  uint64_t next_fight_id_;
  std::map<uint64_t, Fight> fight_storage_;
  work_with_datbase::DBManager dbManager_;
  serverConfigHandler configHandler;
};
