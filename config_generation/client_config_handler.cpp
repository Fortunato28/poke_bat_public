#include "client_config_handler.h"
#include "utilities.h"

using namespace poke_bat::middleware;
using namespace libconfig;

clientConfigHandler::clientConfigHandler()
{
    configFileName_ = "config.cfg";
}

bool clientConfigHandler::IsConfigExist()
{
    std::ifstream config(configFileName_);
    return config.good();
}

void clientConfigHandler::GetDefaultConfig(std::function<void(std::string&)> callback_to_get_from_server)
{
    callback_to_get_from_server(configContent_);
    //printf("HERE %lu\n", configContent_.length());
}

void clientConfigHandler::SaveConfigToFile()
{
    std::ofstream fout;
    fout.open(configFileName_, std::ios_base::trunc);
    if(fout.is_open())
    {
        fout << configContent_;
        fout.close();
    }
    else
    {
        throw std::runtime_error("Can't open file for writing! Try to use sudo.");
    }
}


void clientConfigHandler::LoadConfigFromFile()
{
    std::ifstream fin(configFileName_);
    if(fin.is_open())
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        configContent_ = ss.str();
        fin.close();
    }
    else
    {
        throw std::runtime_error("Can't open file for reading! Try to use sudo.");
    }
}

Pokemon clientConfigHandler::ParseConfig()
{
    Config cfg;
    Pokemon pokemon;
    
    try
    {
        cfg.readString(configContent_);
        pokemon.__set_name(cfg.lookup("pokemon.name"));

        std::string type = cfg.lookup("pokemon.type");
        pokemon.__set_type(utilities::get_enum_poketype(type));

        int HP = cfg.lookup("pokemon.HP");
        pokemon.__set_HP(HP);
        
        int attack = cfg.lookup("pokemon.Attack");       
        pokemon.__set_attack(attack);
        
        int defense = cfg.lookup("pokemon.Defense");       
        pokemon.__set_defense(defense);
        
        int sp_atk = cfg.lookup("pokemon.Sp_Atk");       
        pokemon.__set_spell_attack(sp_atk);
        
        int sp_def = cfg.lookup("pokemon.Sp_Def");       
        pokemon.__set_spell_defense(sp_def);
        
        int speed = cfg.lookup("pokemon.Speed");       
        pokemon.__set_speed(speed);
        
        int EXP = cfg.lookup("pokemon.EXP");       
        pokemon.__set_EXP(EXP);
        
        int LVL = cfg.lookup("pokemon.LVL");       
        pokemon.__set_LVL(LVL);
       
        std::string flag = cfg.lookup("signature.key");
        pokemon.__set_flag(flag);

        const libconfig::Setting& root = cfg.getRoot();
        const libconfig::Setting& skills = root["pokemon"]["skills"];
        const libconfig::Setting& skill = skills[0];
        std::string skill_name = skill[0];
        std::string skill_type = skill[1];
        int skill_amount = skill[2];
        
        PokemonSkill pokemonSkill;
        pokemonSkill.__set_name(skill_name);
        pokemonSkill.__set_type(utilities::get_enum_pokeskilltype(skill_type));
        pokemonSkill.__set_amount(skill_amount);

        pokemon.__set_skill(pokemonSkill); 
    }
    catch (const libconfig::ParseException &pex)
    {
        throw std::runtime_error("String parsing error!");
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        throw std::runtime_error("Label searching error!");
    }
    catch (const libconfig::SettingTypeException &sex)
    {
        throw std::runtime_error("Parameter type error!");
    }

    return pokemon;
}

void clientConfigHandler::UpdateConfig(Pokemon pokemon)
{
    Config cfg;

    try
    {
        cfg.readFile(configFileName_.c_str());

        Setting& _HP = cfg.lookup("pokemon.HP");
        _HP = int(pokemon.HP);

        Setting& _attack = cfg.lookup("pokemon.Attack");
        _attack = int(pokemon.attack);

        Setting& _defense = cfg.lookup("pokemon.Defense");
        _defense = int(pokemon.defense);

        Setting& _sp_atk = cfg.lookup("pokemon.Sp_Atk");
        _sp_atk = int(pokemon.spell_attack);

        Setting& _sp_def = cfg.lookup("pokemon.Sp_Def");
        _sp_def = int(pokemon.spell_defense);

        Setting& _EXP = cfg.lookup("pokemon.EXP");
        _EXP = int(pokemon.EXP);

        Setting& _LVL = cfg.lookup("pokemon.LVL");
        _LVL = int(pokemon.LVL);

        Setting& _key = cfg.lookup("signature.key");
        _key = pokemon.flag;

        cfg.writeFile(configFileName_.c_str());
    }
    catch (const libconfig::ParseException &pex)
    {
        throw std::runtime_error("String parsing error!");
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        throw std::runtime_error("Label searching error!");
    }
    catch (const libconfig::SettingTypeException &sex)
    {
        throw std::runtime_error("Parameter type error!");
    }
}
