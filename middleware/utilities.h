#pragma once

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

    std::map<poke_bat::middleware::PokemonType::type, std::string> invertedPokeTypes
    {
        {poke_bat::middleware::PokemonType::NORMAL,  "NORMAL"   },
        {poke_bat::middleware::PokemonType::FIRE,    "FIRE"     },
        {poke_bat::middleware::PokemonType::WATER,   "WATER"    },
        {poke_bat::middleware::PokemonType::GRASS,   "GRASS"    },
        {poke_bat::middleware::PokemonType::ELECTRIC,"ELECTRIC" },
        {poke_bat::middleware::PokemonType::ICE,     "ICE"      },
        {poke_bat::middleware::PokemonType::FIGHTING,"FIGHTING" },
        {poke_bat::middleware::PokemonType::POISON,  "POISON"   },
        {poke_bat::middleware::PokemonType::GROUND,  "GROUND"   },
        {poke_bat::middleware::PokemonType::FLYING,  "FLYING"   },
        {poke_bat::middleware::PokemonType::PSYCHIC, "PSYCHIC"  },
        {poke_bat::middleware::PokemonType::BUG,     "BUG"      },
        {poke_bat::middleware::PokemonType::ROCK,    "ROCK"     },
        {poke_bat::middleware::PokemonType::GHOST,   "GHOST"    },
        {poke_bat::middleware::PokemonType::DARK,    "DARK"     },
        {poke_bat::middleware::PokemonType::DRAGON,  "DRAGON"   },
        {poke_bat::middleware::PokemonType::STEEL,   "STEEL"    },
        {poke_bat::middleware::PokemonType::FAIRY,   "FAIRY"    }
    };

    // TODO обработка ошибок!
    poke_bat::middleware::PokemonType::type get_enum_poketype(std::string pokemon_type)
    {
        // Upcase all word
        for (auto & c: pokemon_type)
        {
            c = toupper(c);
        }

        return pokeTypes.find(pokemon_type)->second;
    }

    std::string get_string_poketype(poke_bat::middleware::PokemonType::type skill_type)
    {
        return invertedPokeTypes.find(skill_type)->second;
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
        // Upcase all word
        for (auto & c: skill_type)
        {
            c = toupper(c);
        }

        return pokeSkillTypes.find(skill_type)->second;
    }
}
