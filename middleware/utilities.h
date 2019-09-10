#include <map>
#include <string>
#include "gen-cpp/interfaces_types.h"

namespace utilities
{ 
    std::map<std::string, poke_bat::middleware::PokemonType::type> pokeTypes
    {
        {"NORMAL",   poke_bat::middleware::PokemonType::NORMAL},
        {"FIRE",     poke_bat::middleware::PokemonType::FIRE},
        {"WATER",    poke_bat::middleware::PokemonType::WATER},
        {"GRASS",    poke_bat::middleware::PokemonType::GRASS},
        {"ELECTRIC", poke_bat::middleware::PokemonType::ELECTRIC},
        {"ICE",      poke_bat::middleware::PokemonType::ICE},
        {"FIGHTING", poke_bat::middleware::PokemonType::FIGHTING},
        {"POISON",   poke_bat::middleware::PokemonType::POISON},
        {"GROUND",   poke_bat::middleware::PokemonType::GROUND},
        {"FLYING",   poke_bat::middleware::PokemonType::FLYING},
        {"PSYCHIC",  poke_bat::middleware::PokemonType::PSYCHIC},
        {"BUG",      poke_bat::middleware::PokemonType::BUG},
        {"ROCK",     poke_bat::middleware::PokemonType::ROCK},
        {"GHOST",    poke_bat::middleware::PokemonType::GHOST},
        {"DARK",     poke_bat::middleware::PokemonType::DARK},
        {"DRAGON",   poke_bat::middleware::PokemonType::DRAGON},
        {"STEEL",    poke_bat::middleware::PokemonType::STEEL},
        {"FAIRY",    poke_bat::middleware::PokemonType::FAIRY}
    };

    // TODO обработка ошибок!
    poke_bat::middleware::PokemonType::type get_enum_poketype(std::string pokemon_type)
    {
        return pokeTypes.find(pokemon_type)->second;
    } 

    std::map<std::string, poke_bat::middleware::SkillType::type> pokeSkillTypes
    {
        {"ATTACK", poke_bat::middleware::SkillType::ATTACK},
        {"BUFF",   poke_bat::middleware::SkillType::BUFF},
        {"DEBUFF", poke_bat::middleware::SkillType::DEBUFF},
    };

    // TODO обработка ошибок!
    poke_bat::middleware::SkillType::type get_enum_pokeskilltype(std::string skill_type)
    {
        return pokeSkillTypes.find(skill_type)->second; 
    }
}
