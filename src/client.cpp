#include <iostream>
#include <functional>

#include "server_controller.h"
#include "client_controller.h"
#include "client_config_handler.h"

using namespace std;

void printRoundData(const RoundResult& result)
{
    printf("ACTION DESCRIPTION = %s\n", result.actionResultDescription.c_str());

    //TODO print info about server pokemon
}

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

Pokemon getPokemonFromConfig(ClientController& client)
{
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

    return client_pokemon;
}

bool isFightStopped(const RoundResult& roundResult_)
{
    printf("CLIENT_POK %ld\n", roundResult_.clientPokemon.HP);
    printf("SERVER %ld\n", roundResult_.serverPokemon.HP);
    if (roundResult_.clientPokemon.HP <= 0 ||
        roundResult_.serverPokemon.HP <= 0)
    {
        return true;
    }

    return false;
}

Pokemon start_fight()
{
    auto complexity = choose_complexity();

    ClientController thrift_client;
    Pokemon clientPokemon = getPokemonFromConfig(thrift_client);
    FightData fightData;
    thrift_client.startFight(fightData, complexity, clientPokemon);

    // TODO Функция переименована! Подумать, что и как тут нормально принтовать
    //printPokemonData(fightData.pokemon);
    RoundResult roundResult_;

    while(true)
    {
        std::cout << "Press 1 to punch enemy pokemon;\n"
                  << "Press 2 to defend;\n"
                  << "Press 3 to use skill;\n"; 
        int action = 228;
        std::cin >> action;
        std::cout << "=================================\n";

        // TODO нормальная валидация!!!! что вводит только цифры. Если не цифры, то идёт нах
        if(action > 3)
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
    ClientController thrift_client;
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

void client_run()
{
    Pokemon c_pok;
    //TODO nice Pikachu output
    std::cout << "Welcome to PokeBattle!\n";
    while(true)
    {
        //TODO add a nice legend
        std::cout << "Choose your action!\n"
                  << "To start fight press 1;\n"
                  << "To show your current pokemon press 2;\n"
                  << "To save your current pokemon press 3;\n"
                  << "To exit the game press 0;\n";
        int choice;
        std::cin >> choice;
        std::cout << "=================================\n";
        switch(choice)
        {
            case 1:
                std::cout << "Fight is started!\n";
                c_pok = start_fight();
                std::cout << "Fight is over!\n";
                std::cout << "=================================\n";
                break;
            case 2:
                // TODO To think about the way how to pass clientPokemon into method below
                //printPokemonData(clientPokemon);
                std::cout << "Here will be show_pokemon() method\n";
                break;
            case 3:
                save_pokemon(c_pok);
                std::cout << "=================================\n";
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
