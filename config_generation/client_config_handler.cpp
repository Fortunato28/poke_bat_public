#include "client_config_handler.h"
#include "utilities.h"

using namespace poke_bat::middleware;

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
    //TODO sending a request for the new config, recieving it and saving
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

/*
 * Method load_config_from_file() loads data from file in a old fashioned C way
 * because otherwise there is an uncertain lack of encrypted bytes.
 * After reading from file this method converts data into the std::string.
 */

void clientConfigHandler::LoadConfigFromFile()
{
    FILE * ptrFile = fopen(configFileName_.c_str(), "rb");

    if (ptrFile ==  NULL)
    {
        throw std::runtime_error("Can't open file for reading!");
    }

    fseek(ptrFile, 0, SEEK_END);
    long iSize = ftell(ptrFile);
    rewind(ptrFile);
    char*
        buffer = (char*) malloc(sizeof(char) * iSize);
    size_t result = fread(buffer, 1, iSize, ptrFile);

    std::string temp(buffer, result);
    configContent_ = temp;

    fclose(ptrFile);
    free(buffer);
}

void clientConfigHandler::DecryptConfig()
{ 
    //conversion data into char 
    char *bufferContent = new char[configContent_.length() + 1];
    for (int i = 0; i < configContent_.length(); ++i)
    {
        bufferContent[i] = configContent_.c_str()[i];
    }
    bufferContent[configContent_.length()] = '\0';

    char password[] = "12345678";

    char salt[] = "12345678";

    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    size_t bufferContentLength = configContent_.length();
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
    configContent_ = temp;

    delete [] bufferContent;
    free(buffer);
}

Pokemon clientConfigHandler::ParseConfig()
{
    libconfig::Config cfg;
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
