/* 
 * This header contain the class which will encrypt and store pokemon data. 
 * Nothing special, really ¯\_(ツ)_/¯.
*/
#pragma once

#include <string>
#include <vector>
#include "picosha2.h"

#include "gen-cpp/interfaces_types.h"

class serverConfigHandler
{
public:
    serverConfigHandler();
    std::string SignConfig();
    bool isSignatureValid(poke_bat::middleware::Pokemon pokemon);
    void ResignPokemon(poke_bat::middleware::Pokemon& pokemon);

private:
    std::string salt;
    std::string takeSignature(std::string content);
};
