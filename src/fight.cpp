#include "fight.h"

Fight::Fight(Pokemon client_pokemon, Pokemon server_pokemon) :
    clientState_(client_pokemon),
    serverState_(server_pokemon)
{
}

Pokemon& Fight::getClientPok()
{
    return clientState_.pokemon_;
}

Pokemon& Fight::getServerPok()
{
    return serverState_.pokemon_;
}

void Fight::setClientDefense()
{
    clientState_.setDefense();
}

void Fight::setServerDefense()
{
    serverState_.setDefense();
}

void Fight::handleClientStats()
{
    printf("HERE def BEFORE %ld\n", clientState_.pokemon_.defense);
    if(clientState_.isInDefense_)
    {
        clientState_.isInDefense_ = false;
        clientState_.pokemon_.defense /= 2;
    }

    printf("HERE def AFTER %ld\n", clientState_.pokemon_.defense);
}

void Fight::handleServerStats()
{
    if(serverState_.isInDefense_)
    {
        serverState_.isInDefense_ = false;
        serverState_.pokemon_.defense /= 2;
    }
}

Fight::PokemonState::PokemonState(Pokemon pok):
    pokemon_(pok),
    isInDefense_(false),
    buf_(0),
    bufRoundCounter_(0),
    debuf_(0),
    debufRoundCounter_(0)
{}

void Fight::PokemonState::setDefense()
{
    isInDefense_ = true;
    pokemon_.defense *= 2;
}
