#pragma once

#include <map>
#include <string>
#include "gen-cpp/interfaces_types.h"

namespace utilities
{
    poke_bat::middleware::PokemonType::type get_enum_poketype(std::string pokemon_type);
    std::string get_string_poketype(poke_bat::middleware::PokemonType::type pokemon_type);
    poke_bat::middleware::SkillType::type get_enum_pokeskilltype(std::string skill_type);
    std::string get_string_pokeskilltype(poke_bat::middleware::SkillType::type skill_type);
}
