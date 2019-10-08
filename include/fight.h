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
    void decreaseClientHPDueToPunch();
    void decreaseClientHPDueToSkill();

    void decreaseServerHPDueToPunch();
    void decreaseServerHPDueToSkill();

    void setClientDefense();
    void setServerDefense();

    void setClientBuf();
    void setServerBuf();

    void setClientDebuf();
    void setServerDebuf();

    void toggleLastActionPunch();

    // Handlers
    void handleClientStats();
    void handleServerStats();

    // Getters
    bool isServerLowHP();
    bool isServerBuffed();
    bool isClientDebuffed();
    bool isLastActionPunch();
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
            int bufRoundCounter_;

            uint64_t debuf_;
            int debufRoundCounter_;
            int64_t defaultAttack_;

            // Для выбора действий сервера на основе процента оставшихся ХП
            uint64_t defaultHP_;
    };

private:
    PokemonState clientState_;
    PokemonState serverState_;;
    bool lastActionPunch_;
};
