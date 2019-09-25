#include "fight.h"

Fight::Fight(Pokemon client_pokemon, Pokemon server_pokemon) : client_pokemon_(client_pokemon), server_pokemon_(server_pokemon)
{
}

Fight::PokemonState::PokemonState():
    def(0),
    buf(0),
    bufRoundCounter(0),
    debuf(0),
    debufRoundCounter(0)
{}
