#pragma once 
#include <iostream>
#include "gen-cpp/PokServer.h"

using Pokemon = poke_bat::middleware::Pokemon;

class Fight
{
public:
    //TODO remove excess copy
    Fight(Pokemon client_pokemon, Pokemon server_pokemon);
    Pokemon client_pokemon_;
    Pokemon server_pokemon_;
private:

    class PokemonState
    {
        friend class Fight;
        PokemonState();

        uint64_t def;

        uint64_t buf;
        uint32_t bufRoundCounter;

        uint64_t debuf;
        uint32_t debufRoundCounter;
    };

    PokemonState clientState_;
    PokemonState serverState_;;
};
