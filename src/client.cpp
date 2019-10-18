#include <iostream>
#include <functional>

#include "server_controller.h"
#include "client_controller.h"
#include "client_config_handler.h"
#include "utilities.h"

using namespace std;

static string server_address_;

void printPokemon(const Pokemon& pok)
{
    printf("POKEMON`S NAME = %s\n", pok.name.c_str());
    printf("POKEMON`S TYPE = %s\n", utilities::get_string_poketype(pok.type).c_str());
    printf("POKEMON`S HP = %ld\n", pok.HP);
    printf("POKEMON`S ATTACK = %ld\n", pok.attack);
    printf("POKEMON`S DEFENSE = %ld\n", pok.defense);
    printf("POKEMON`S SPELL TYPE = %s\n", pok.skill.name.c_str());
    printf("POKEMON`S SPELL ATTACK = %ld\n", pok.spell_attack);
    printf("POKEMON`S SPELL DEFENSE = %ld\n", pok.spell_defense);
    printf("POKEMON`S EXPERIENCE = %ld\n", pok.EXP);
    printf("POKEMON`S LEVEL = %ld\n", pok.LVL);
}

void printRoundData(const RoundResult& result)
{
    printf("Your pokemon:\n");
    printPokemon(result.clientPokemon);
    printf("\n");
    printf("Opponent`s pokemon:\n");
    printPokemon(result.serverPokemon);

    printf("%s\n", result.actionResultDescription.c_str());
}

Pokemon getPokemonFromConfig(ClientController& client)
{
    clientConfigHandler config_handler(server_address_);
    if(!config_handler.IsConfigExist())
    {
        config_handler.GetDefaultConfig([&client](std::string& encrypted_config){
                client.getConfig(encrypted_config);
                });
        config_handler.SaveConfigToFile();
    }
    config_handler.LoadConfigFromFile();

    Pokemon client_pokemon = config_handler.ParseConfig();

    return client_pokemon;
}

bool isFightStopped(const RoundResult& roundResult_)
{
    if (roundResult_.clientPokemon.HP <= 0 ||
        roundResult_.serverPokemon.HP <= 0)
    {
        return true;
    }

    return false;
}

const string choose_opponent()
{
    string entered_id;
    cout << "Enter the ID of the pokemon you wanna fight with:\n";
    cin.ignore();
    std::getline(cin, entered_id);
    return entered_id;
}

Pokemon start_fight()
{
    auto opponent_pub_id = choose_opponent();

    ClientController thrift_client(server_address_);
    Pokemon clientPokemon = getPokemonFromConfig(thrift_client);
    FightData fightData;
    //thrift_client.startFight(fightData, complexity, clientPokemon);
    thrift_client.startFight(fightData, opponent_pub_id, clientPokemon);
    if(fightData.pokemon.EXP == -1)
    {
        cout << "There are no pokemon with id = " << opponent_pub_id << "\n";
        return clientPokemon;
    }

    if (fightData.fight_id == -1)
    {
        throw std::runtime_error("Your signature wasn't valid.\n"
                                 "Now you're going to die!");
    }

    RoundResult roundResult_;
    roundResult_.clientPokemon = clientPokemon;
    roundResult_.serverPokemon = fightData.pokemon;
    roundResult_.actionResultDescription = "Fight is started!\n";
    printRoundData(roundResult_);

    while(true)
    {
        std::cout << "Press 1 to punch enemy pokemon;\n"
                  << "Press 2 to defend;\n"
                  << "Press 3 to use skill;\n"; 
        //int action = 228;
        //std::cin >> action;
        //std::cout << "=================================\n";

        int action;
        string buf;
        getline(cin, buf);
        std::istringstream sstr(buf);
        sstr >> action;

        if(action > 3 || action == 0)
        {
            std::cout << "What are you, fucking immature!?\n"
                << "Try it again!\n";
        }
        else
        {
            switch(action)
            {
                case 1:
                    thrift_client.punch(roundResult_);
                    break;
                case 2:
                    thrift_client.defend(roundResult_);
                    break;
                case 3:
                    thrift_client.useSkill(roundResult_, clientPokemon.skill.name);
                    break;
            }
            if(isFightStopped(roundResult_))
            {
                printf("%s\n", roundResult_.actionResultDescription.c_str());
                std::cout << "=================================\n";
                break;
            }
            printRoundData(roundResult_);
        }

    }
    return roundResult_.clientPokemon;
}

void save_pokemon(Pokemon& c_pok)
{
    ClientController thrift_client(server_address_);
    // Клиентского покемона ещё и нет вовсе (боя ещё не было)
    if(c_pok.name == "")
    {
        c_pok = getPokemonFromConfig(thrift_client);
    }

    std::cout << "Enter private id to find your pokemon later!\n";
    string private_id;
    std::cin >> private_id;

    std::cout << "Enter some comment about your sweety pokemon!\n";
    string comment;
    std::cin >> comment;

    string result = thrift_client.savePokemon(private_id, c_pok, comment);
    cout << result;
}

void show_saved_poks()
{
    ClientController thrift_client(server_address_);
    cout << thrift_client.getSavedPoksTable();
}

void show_pok_by_private_id()
{
    string entered_private_id;
    cout << "Enter the private ID of the pokemon:\n";
    cin.ignore();
    std::getline(cin, entered_private_id);

    ClientController thrift_client(server_address_);
    Pokemon gottenPokemon = thrift_client.getSavedPokByPrivateID(entered_private_id);

    if(gottenPokemon.EXP == -1)
    {
        cout << "There are no pokemon with id = " << entered_private_id << "\n";
        return;
    }

    printPokemon(gottenPokemon);
}

void show_current_pok(Pokemon& c_pok)
{
    ClientController thrift_client(server_address_);
    // Клиентского покемона ещё и нет вовсе (боя ещё не было)
    if(c_pok.name == "")
    {
        c_pok = getPokemonFromConfig(thrift_client);
    }

    printPokemon(c_pok);
}

void client_run()
{
    clientConfigHandler config_handler(server_address_);
    Pokemon c_pok;
    //TODO nice Pikachu output
    std::cout << "Welcome to PokeBattle!\n";
    while(true)
    {
        std::cout << "Choose your action!\n"
                  << "To start fight press 1;\n"
                  << "To save your current pokemon press 2;\n"
                  << "To show your current pokemon press 3;\n"
                  << "To show table with saved pokemons press 4;\n"
                  << "To show saved pokemons by private id press 5;\n"
                  << "To exit the game press 0;\n";
        int choice;
        std::cin >> choice;
        std::cout << "=================================\n";
        switch(choice)
        {
            case 1:
                c_pok = start_fight();
                config_handler.UpdateConfig(c_pok);
                std::cout << "Fight is over!\n";
                std::cout << "=================================\n";
                break;
            case 2:
                save_pokemon(c_pok);
                std::cout << "=================================\n";
                break;
            case 3:
                show_current_pok(c_pok);
                std::cout << "=================================\n";
                break;
            case 4:
                show_saved_poks();
                std::cout << "=================================\n";
                break;
            case 5:
                show_pok_by_private_id();
                std::cout << "=================================\n";
                break;
            case 0:
                return;
        }
    }
}

string check_cmd_params(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Usage:\nclient <server ip>\n";
        throw std::runtime_error("Bad commandline arguments!");
    }

    return {argv[1]};
}

int main(int argc, char* argv[])
{
    server_address_ = check_cmd_params(argc, argv);
    client_run();
    return 0;
}
