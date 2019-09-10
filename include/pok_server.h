#pragma once

#include "gen-cpp/PokServer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using PokServerIf = poke_bat::middleware::PokServerIf;
using Config = poke_bat::middleware::Config;
using Pokemon = poke_bat::middleware::Pokemon;
using RoundResult = poke_bat::middleware::RoundResult;

class PokServerHandler : virtual public PokServerIf
{
 public:
  PokServerHandler();

  void getConfig(Config& _return);

  void startFight(Pokemon& _return, const int64_t complexity, const Pokemon& clientPokemon);

  void punch(RoundResult& _return);

  void defend(RoundResult& _return);

  void useSkill(RoundResult& _return, const std::string& skillName);
};
