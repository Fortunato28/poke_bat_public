#include <iostream>
#include <functional>

#include "server_controller.h"
#include "client_controller.h"
#include "client_config_handler.h"

using namespace std;

const int64_t choose_complexity()
{
    int64_t result;
    std::cout << "Choose the complexity of the fight!\n"
              << "1, 2 or 3.\n";
    std::cin >> result;
    std::cout << "=================================\n";
    switch(result)
    {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case 3:
            return 3;
            break;
        default:
            std::cout << "Your input is bullshit!\n";
            return 3;
    }
}

void start_fight()
{
    auto complexity = choose_complexity();
    ClientController client;

    clientConfigHandler config_handler;
    if(!config_handler.IsConfigExist())
    {
        config_handler.GetDefaultConfig([&client](std::string& encrypted_config){
                client.getConfig(encrypted_config);
                });
        config_handler.SaveConfigToFile();
    }
    config_handler.LoadConfigFromFile();
    config_handler.DecryptConfig();

    Pokemon client_pokemon = config_handler.ParseConfig();
    FightData fightData;
    client.startFight(fightData, complexity, client_pokemon);
    //TODO print info about server pokemon
    RoundResult roundResult_;

    while(true)
    {
        std::cout << "Press 1 to punch enemy pokemon;\n"
                  << "Press 2 to defend;\n"
                  << "Press 3 to use skill;\n"; 
        int action;
        std::cin >> action;
        std::cout << "=================================\n";
        switch(action)
        {
            case 1:
                client.punch(roundResult_);
                break;
            case 2:
                client.defend(roundResult_);
                break;
            case 3:
                client.useSkill(roundResult_, client_pokemon.skill.name);
                break;
            default:
                std::cout << "What are you, fucking immature!?\n"
                          << "Try it again!\n";
        }
    }
}

void client_run()
{
    //TODO nice Pikachu output
    std::cout << "Welcome to PokeBattle!\n";
    while(true)
    {
        //TODO add a nice legend
        std::cout << "Choose your action!\n"
                  << "To start press 1;\n"
                  << "To exit the game press 0;\n";
        int choice;
        std::cin >> choice;
        std::cout << "=================================\n";
        switch(choice)
        {
            case 1:
                std::cout << "Fight is started!\n";
                start_fight();
                std::cout << "Fight is over!\n";
                break;
            case 2:
                std::cout << "2\n";
                break;
            case 0:
                return;
        }
    }
}


int main()
{
    //TODO get the IP of the server
    //TODO throw it in thrift client
    client_run();
    return 0;
}
