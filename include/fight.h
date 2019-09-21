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
};
