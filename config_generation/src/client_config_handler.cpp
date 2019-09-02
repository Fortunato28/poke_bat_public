#include "client_config_handler.h"

using namespace poke_bat::middleware;

bool clientConfigHandler::IsConfigExist()
{
    std::ifstream config("config.cfg");
    return config.good(); 
}

void clientConfigHandler::GetDefaultConfig()
{
    //TODO sending a request for the new config, recieving it and saving
}

void clientConfigHandler::SaveConfigToFile()
{
    std::ofstream fout("config.cfg", std::ios_base::trunc);
    fout << configContent;
    fout.close(); 
}

/*
 * Method load_config_from_file() loads data from file in a old fashioned C way
 * because otherwise there is an uncertain lack of encrypted bytes.
 * After reading from file this method converts data into the std::string.
 */

void clientConfigHandler::LoadConfigFromFile()
{
    FILE * ptrFile = fopen("config.cfg", "rb");

    if (ptrFile ==  NULL)
    {
        printf("Can't open file!\n");
        exit(1);
    }

    fseek(ptrFile, 0, SEEK_END);
    long iSize = ftell(ptrFile);
    rewind(ptrFile);
    char* buffer = (char*) malloc(sizeof(char) * iSize);
    size_t result = fread(buffer, 1, iSize, ptrFile);

    std::string temp(buffer, result);
    configContent = temp;

    fclose(ptrFile);
    free(buffer);
}

void clientConfigHandler::DecryptConfig()
{ 
    //conversion data into char 
    char *bufferContent = new char[configContent.length() + 1];
    for (int i = 0; i < configContent.length(); ++i)
    {
        bufferContent[i] = configContent.c_str()[i];
    }
    bufferContent[configContent.length()] = '\0';

    char password[] = "12345678";

    char salt[] = "12345678";

    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    size_t bufferContentLength = configContent.length();
    size_t passwordLength = strlen(password);
    size_t saltLength = strlen(salt);
    char* buffer = (char*)malloc(bufferContentLength);

    //crypto-descryptor initialisation
    gcryError = gcry_cipher_open(&descriptorPointer,
                                 GCRY_CIPHER_DES,
                                 GCRY_CIPHER_MODE_CBC,
                                 GCRY_CIPHER_CBC_CTS);
    if (gcryError) {
        printf("gcry_cipher_open failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //set key for the decryption
    gcryError = gcry_cipher_setkey(descriptorPointer, password, passwordLength);
    if (gcryError) {
        printf("gcry_cipher_setkey failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //set salt for the decryption
    gcryError = gcry_cipher_setiv(descriptorPointer, salt, saltLength);
    if (gcryError) {
        printf("gcry_cipher_setiv failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //decryption
    gcryError = gcry_cipher_decrypt(descriptorPointer, 
                                    buffer, 
                                    bufferContentLength, 
                                    bufferContent, 
                                    bufferContentLength);
    if (gcryError) {
        printf("gcry_cipher_encrypt failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //closing crypto-descryptor
    gcry_cipher_close(descriptorPointer);

    //conversion data back to std::string
    std::string temp(buffer, bufferContentLength);
    configContent = temp;

    delete [] bufferContent;
    free(buffer);
}

Pokemon clientConfigHandler::ParseConfig()
{
    libconfig::Config cfg;
    Pokemon pokemon;
    
    try
    {
        cfg.readString(configContent);
        pokemon.__set_name(cfg.lookup("pokemon.name"));
       
        std::string type = cfg.lookup("pokemon.type"); 
        pokemon.__set_type(utilities::pokeTypes.find(type)->second);

        pokemon.__set_HP(cfg.lookup("pokemon.HP"));
        pokemon.__set_attack(cfg.lookup("pokemon.Attack"));
        pokemon.__set_defense(cfg.lookup("pokemon.Defense"));
        pokemon.__set_spell_attack(cfg.lookup("pokemon.Sp_Atk"));
        pokemon.__set_spell_defense(cfg.lookup("pokemon.Sp_Def"));
        pokemon.__set_speed(cfg.lookup("pokemon.Speed"));
        pokemon.__set_EXP(cfg.lookup("pokemon.EXP"));
        pokemon.__set_LVL(cfg.lookup("pokemon.LVL"));
       
        const libconfig::Setting& root = cfg.getRoot();
        const libconfig::Setting& skills = root["pokemon"]["skills"];
        const libconfig::Setting& skill = skills[0];
        std::string skill_name = skill[0];
        std::string skill_type = skill[1];
        int skill_amount = skill[2];
        
        PokemonSkill pokemonSkill;
        pokemonSkill.__set_name(skill_name);
        pokemonSkill.__set_type(utilities::pokeSkillTypes.find(skill_type)->second);
        pokemonSkill.__set_amount(skill_amount);

        pokemon.__set_skill(pokemonSkill); 
    } 
    catch (const libconfig::ParseException &pex)
    {
        printf("String parsing error!\n");
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        printf("Label searching error!\n");
    }

    return pokemon;
}
