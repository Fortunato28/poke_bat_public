#pragma once 
#include <iostream>
#include "gen-cpp/PokServer.h"

using Pokemon = poke_bat::middleware::Pokemon;

class Fight
{
public:
    //TODO remove excess copy
    Fight(Pokemon client_pokemon, Pokemon server_pokemon);

    // Getters
    Pokemon& getClientPok();
    Pokemon& getServerPok();

    // Setters
    void setClientDefense();
    void setServerDefense();

    // Handlers
    void handleClientStats();
    void handleServerStats();
private:

    class PokemonState
    {
        public:
            void setDefense();
        private:
            Pokemon pokemon_;
            friend class Fight;
            PokemonState(Pokemon pok);

            bool isInDefense_;

            uint64_t buf_;
            uint32_t bufRoundCounter_;

            uint64_t debuf_;
            uint32_t debufRoundCounter_;
    };

private:
    PokemonState clientState_;
    PokemonState serverState_;;
};
