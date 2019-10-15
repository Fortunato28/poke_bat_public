#include <iostream>
#include <functional>

#include "server_controller.h"
#include "client_controller.h"
#include "client_config_handler.h"

using namespace std;

void printPokemon(const Pokemon& pok)
{
    // TODO implement
    printf("POKEMON`S NAME = %s\n", pok.name.c_str());
    printf("PUB ID = %s\n", pok.pub_id.c_str());
}

void printRoundData(const RoundResult& result)
{
    printf("ACTION DESCRIPTION = %s\n", result.actionResultDescription.c_str());

    //TODO print info about server pokemon
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

    ClientController thrift_client;
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

void show_saved_poks()
{
    ClientController thrift_client;
    cout << thrift_client.getSavedPoksTable();
}

void show_pok_by_private_id()
{
    string entered_private_id;
    cout << "Enter the private ID of the pokemon:\n";
    cin.ignore();
    std::getline(cin, entered_private_id);

    ClientController thrift_client;
    Pokemon gottenPokemon = thrift_client.getSavedPokByPrivateID(entered_private_id);

    if(gottenPokemon.EXP == -1)
    {
        cout << "There are no pokemon with id = " << entered_private_id << "\n";
        return;
    }

    printPokemon(gottenPokemon);
}

void client_run()
{
    clientConfigHandler config_handler;
    Pokemon c_pok;
    //TODO nice Pikachu output
    std::cout << "Welcome to PokeBattle!\n";
    while(true)
    {
        //TODO add a nice legend
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
                std::cout << "Fight is started!\n";
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
                // TODO To think about the way how to pass clientPokemon into method below
                //printPokemonData(clientPokemon);
                std::cout << "Here will be show_pokemon() method\n";
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


int main()
{
    //TODO get the IP of the server
    //TODO throw it in thrift client
    client_run();
    return 0;
}
