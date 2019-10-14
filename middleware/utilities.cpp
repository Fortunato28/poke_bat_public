#include "utilities.h"

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

    std::map<std::string, poke_bat::middleware::SkillType::type> pokeSkillTypes
    {
        {"ATTACK", poke_bat::middleware::SkillType::ATTACK},
        {"BUFF",   poke_bat::middleware::SkillType::BUFF},
        {"DEBUFF", poke_bat::middleware::SkillType::DEBUFF},
    };

    std::map<poke_bat::middleware::SkillType::type, std::string> invertedPokeSkillTypes
    {
        {poke_bat::middleware::SkillType::ATTACK, "ATTACK"},
        {poke_bat::middleware::SkillType::BUFF, "BUFF"},
        {poke_bat::middleware::SkillType::DEBUFF, "DEBUFF"},
    };

    poke_bat::middleware::PokemonType::type get_enum_poketype(std::string pokemon_type);
    std::string get_string_poketype(poke_bat::middleware::PokemonType::type pokemon_type);
    poke_bat::middleware::SkillType::type get_enum_pokeskilltype(std::string skill_type);
    std::string get_string_pokeskilltype(poke_bat::middleware::SkillType::type skill_type);
    // TODO обработка ошибок!
    poke_bat::middleware::PokemonType::type get_enum_poketype(std::string pokemon_type)
    {
        // Uppercase all words
        for (auto & c: pokemon_type)
        {
            c = toupper(c);
        }

        return pokeTypes.find(pokemon_type)->second;
    }

    std::string get_string_poketype(poke_bat::middleware::PokemonType::type pokemon_type)
    {
        return invertedPokeTypes.find(pokemon_type)->second;
    }


    // TODO обработка ошибок!
    poke_bat::middleware::SkillType::type get_enum_pokeskilltype(std::string skill_type)
    {
        // Uppercase all words
        for (auto & c: skill_type)
        {
            c = toupper(c);
        }

        return pokeSkillTypes.find(skill_type)->second;
    }

    std::string get_string_pokeskilltype(poke_bat::middleware::SkillType::type skill_type)
    {
        return invertedPokeSkillTypes.find(skill_type)->second;
    }
}
