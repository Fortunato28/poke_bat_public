#include "server_config_handler.h"
#include "utilities.h"

using namespace poke_bat::middleware;

void bindValue(std::string& content,
               const std::string what,
               const std::string whithWhat)
{
    size_t position = content.find(what);
    if (position == std::string::npos)
    {
        return;
    }
    content.replace(position, what.length(), whithWhat);
}

serverConfigHandler::serverConfigHandler()
{
    std::string salt = "GonnaHASH'emall";
}

std::string serverConfigHandler::SignConfig()
{
    std::string content = "pokemon:\n"
        "{\n"
            "\tname = \"picachu\";\n"
            "\ttype = \"ELECTRIC\";\n"
            "\tHP = 35;\n"
            "\tAttack = 55;\n"
            "\tDefense = 30;\n"
            "\tSp_Atk = 50;\n"
            "\tSp_Def = 40;\n"
            "\tSpeed = 90;\n"
            "\tEXP = 0;\n"
            "\tLVL = 1;\n"
            "\tskills = ( (\"lightning bow\","
                        "\"ATTACK\","
                        "4) );\n"
        "};\n"
        "signature:\n"
        "{\n"
            "\tkey = \"\"\n"
        "};";

    std::string forHash = content + salt;

    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(forHash.begin(), forHash.end(),
                      hash.begin(), hash.end());

    std::string signature = picosha2::bytes_to_hex_string(hash.begin(),
                                                          hash.end());
    size_t position = content.find_last_of('\"');
    content.insert(position, signature);

    return content;
}

bool serverConfigHandler::isSignatureValid(Pokemon pokemon)
{
    //get pokemon stats
    std::string name = pokemon.name;
    std::string type = utilities::get_string_poketype(pokemon.type);
    std::string HP = std::to_string(pokemon.HP);
    std::string Attack = std::to_string(pokemon.attack);
    std::string Defense = std::to_string(pokemon.defense);
    std::string Sp_Atk = std::to_string(pokemon.spell_attack);
    std::string Sp_Def = std::to_string(pokemon.spell_defense);
    std::string Speed = std::to_string(pokemon.speed);
    std::string EXP = std::to_string(pokemon.EXP);
    std::string LVL = std::to_string(pokemon.LVL);
    std::string skills_name = pokemon.skill.name;
    std::string skills_type = utilities::get_string_pokeskilltype(pokemon.skill.type);
    std::string skills_amount = std::to_string(pokemon.skill.amount);

    //form a string
    std::string gottenContent = "pokemon:\n"
        "{\n"
            "\tname = \"/!\";\n"
            "\ttype = \"/!\";\n"
            "\tHP = /!;\n"
            "\tAttack = /!;\n"
            "\tDefense = /!;\n"
            "\tSp_Atk = /!;\n"
            "\tSp_Def = /!;\n"
            "\tSpeed = /!;\n"
            "\tEXP = /!;\n"
            "\tLVL = /!;\n"
            "\tskills = ( (\"/!\","
                        "\"/!\","
                        "/!) );\n"
        "};\n"
        "signature:\n"
        "{\n"
            "\tkey = \"\"\n"
        "};";

    bindValue(gottenContent, "/!", name);
    bindValue(gottenContent, "/!", type);
    bindValue(gottenContent, "/!", HP);
    bindValue(gottenContent, "/!", Attack);
    bindValue(gottenContent, "/!", Defense);
    bindValue(gottenContent, "/!", Sp_Atk);
    bindValue(gottenContent, "/!", Sp_Def);
    bindValue(gottenContent, "/!", Speed);
    bindValue(gottenContent, "/!", EXP);
    bindValue(gottenContent, "/!", LVL);
    bindValue(gottenContent, "/!", skills_name);
    bindValue(gottenContent, "/!", skills_type);
    bindValue(gottenContent, "/!", skills_amount);

    //take a signature
    std::string gottenSignature = pokemon.flag;

    //take a basic signature
    std::string forBasicHash = gottenContent + salt;
    std::vector<unsigned char> basicHash(picosha2::k_digest_size);
    picosha2::hash256(forBasicHash.begin(), forBasicHash.end(),
                      basicHash.begin(), basicHash.end());
    std::string basicSignature = picosha2::bytes_to_hex_string(
                      basicHash.begin(), basicHash.end());

    //match 'em
    if (gottenSignature == basicSignature)
    {
        return true;
    }
    else
    {
        return false;
    }
}
