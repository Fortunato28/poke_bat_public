// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "pok_server.h"
#include "gen-cpp/PokServer.h"
#include "fight.h"
#include "db_conf.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace ::poke_bat::middleware;
using namespace work_with_datbase;

PokServerHandler::PokServerHandler() 
    : next_fight_id_(0),
      dbManager_(host, user, pass, db)
{
}

static std::string bce(const std::string &in)
{
    std::string out;

    int val=0, valb=-6;
    for (unsigned char c : in)
    {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) 
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x2F]);
            valb-=6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x2F]);
    while (out.size()%5) out.push_back('=');
    return out;
}

std::string get_pub_id(const std::string& private_id)
{
    return bce(private_id);
}

void PokServerHandler::getSavedPoksTable(std::string& _return)
{
    _return = dbManager_.GetSavedPoks();
}

void PokServerHandler::getSavedPokByPrivateID(Pokemon& _return, const std::string& private_id)
{
    _return = dbManager_.GetPokByPrivateID(private_id);
}

void PokServerHandler::savePokemon(std::string& _return, const std::string& private_id, const Pokemon& client_pokemon, const std::string& comment)
{
    std::string pub_id = get_pub_id(private_id);
    _return = dbManager_.SavePokemon(private_id, pub_id, client_pokemon, comment);
}

Fight& PokServerHandler::findFight(const int64_t &fight_id)
{
    //printf("%lu", fight_storage.find(fight_id)->first);
    return fight_storage_.at(fight_id);
}

void serverPunch(Fight& current_fight, RoundResult& _return)
{
    current_fight.decreaseClientHPDueToPunch();

    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
    _return.__set_actionResultDescription(_return.actionResultDescription +
            "Opponent`s pokemon punched your pokemon. Suffer (っಠ‿ಠ)っ.");
}

void serverDefense(Fight& current_fight, RoundResult& _return)
{
    current_fight.setServerDefense();

    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
    _return.__set_actionResultDescription(_return.actionResultDescription +
            "Opponent`s pokemon set block. Try to penetrate ( ͡° ͜ʖ ͡° ).");
}

void serverUseSkill(Fight& current_fight, RoundResult& _return)
{
    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();


    const PokemonSkill& s_skill = s_pok.skill;
    switch(s_skill.type)
    {
        case SkillType::BUFF:
            // Buff client pok
            current_fight.setServerBuf();
            break;
        case SkillType::DEBUFF:
            // Debuff client pok
            current_fight.setClientDebuf();
            break;
        case SkillType::ATTACK:
            // Decrease server pok HP
            current_fight.decreaseClientHPDueToSkill();
            break;
    }

    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
    _return.__set_actionResultDescription(
            _return.actionResultDescription +
            "Opponent`s pokemon used skill " +
            s_pok.skill.name.c_str() +
            ". You'll like it ╰( ͡° ͜ʖ ͡°  )つ──☆*:・ﾟ");
}

void serverAction(Fight& current_fight, RoundResult& _return)
{
    current_fight.handleServerStats();
    auto& s_pok = current_fight.getServerPok();

    // Server pokemons AI
    if(!current_fight.isServerLowHP())
    {
        if(s_pok.skill.amount)
        {
            if(s_pok.skill.type == SkillType::BUFF &&
                    !current_fight.isServerBuffed())
            {
                serverUseSkill(current_fight, _return);
                return;
            }
            if(s_pok.skill.type == SkillType::DEBUFF &&
                    !current_fight.isClientDebuffed())
            {
                serverUseSkill(current_fight, _return);

                return;
            }
            if(s_pok.skill.type == SkillType::ATTACK &&
                    current_fight.isLastActionPunch())
            {
                serverUseSkill(current_fight, _return);
                current_fight.toggleLastActionPunch();
                return;
            }
            else
            {
                serverPunch(current_fight, _return);
                current_fight.toggleLastActionPunch();
                return;
            }
        }
        else
        {
            _return.__set_actionResultDescription( _return.actionResultDescription + "Opponent`s pokemon is tired, so it can only do punch.\n");
            serverPunch(current_fight, _return);
            return;
        }
    }
    else
    {
        if(current_fight.isLastActionPunch())
        {
            serverDefense(current_fight, _return);
            current_fight.toggleLastActionPunch();
            return;
        }
        else
        {
            serverPunch(current_fight, _return);
            current_fight.toggleLastActionPunch();
            return;
        }
    }
}

void PokServerHandler::getConfig(std::string& _return)
{
    _return = configHandler.SignConfig();
}

size_t fibonacci(size_t border)
{
    size_t t1 = 0;
    size_t t2 = 1;
    size_t nextTerm = 0;
    for (int i = 1; i <= border; ++i)
    {
        // Prints the first two terms.
        if(i == 1)
        {
            continue;
        }
        if(i == 2)
        {
            continue;
        }
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;

    }
    return t2;
}

// TODO обработка ошибки, а вдруг нет такого покемоноса
void PokServerHandler::startFight(FightData& _return, const std::string& pub_id, const Pokemon& clientPokemon)
{
    if (!configHandler.isSignatureValid(clientPokemon))
    {
        _return.__set_fight_id(-1);
        return;
    }

    _return.pokemon = dbManager_.GetPokemonToFight(pub_id);
    Fight fight(clientPokemon, _return.pokemon);
    fight_storage_.emplace(next_fight_id_, fight);

    _return.__set_fight_id(next_fight_id_);
    ++next_fight_id_;
}

std::string clientWin()
{
    return {"Congratulations!\n"
            "You won the battle!\n"
            "You earn some EXP!\n"};
}

std::string serverWin()
{
    return {"Try harder next time, loser!\n"
            "You lost the battle!\n"
            "Your pokemon is dead inside!\n"};
}

bool isDeadInside(const Pokemon& pok)
{
    return pok.HP <= 0 ? true : false;
}

void PokServerHandler::lvlUp(Fight& current_fight)
{
    auto& c_pok = current_fight.getClientPok();

    c_pok.HP += 15;
    c_pok.attack += 3;
    c_pok.defense += 3;
    c_pok.spell_attack += 9;
    c_pok.spell_defense += 9;
    c_pok.LVL++;
}

bool PokServerHandler::isLvlUp(Pokemon& pok)
{
    return (pok.EXP >= (fibonacci(pok.LVL + 3) * 100)) ? true : false;
}

bool PokServerHandler::isFightStopped(
        RoundResult& roundResult_,
        Fight& current_fight,
        const int64_t fight_id)
{

    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    if(isDeadInside(s_pok))
    {
        //TODO calculate exp according to s_pok level
        c_pok.HP = current_fight.GetDefaultClienHP();
        c_pok.EXP += s_pok.LVL * 100;
        c_pok.skill.amount = 5;
        if(isLvlUp(c_pok))
        {
            lvlUp(current_fight);
        }
        configHandler.ResignPokemon(c_pok);

        roundResult_.__set_clientPokemon(c_pok);

        roundResult_.__set_serverPokemon(s_pok);
        roundResult_.__set_actionResultDescription(clientWin() + dbManager_.GetComment(s_pok.pub_id) + "\n");
        // Drop fight
        fight_storage_.erase(fight_id);
        return true;
    }
    if(isDeadInside(c_pok))
    {
        //TODO прокачка серверного покемона
        roundResult_.__set_clientPokemon(c_pok);
        roundResult_.__set_serverPokemon(s_pok);
        roundResult_.__set_actionResultDescription(serverWin());
        // Drop fight
        fight_storage_.erase(fight_id);
        return true;
    }

    return false;
}

void PokServerHandler::punch(RoundResult& _return, const int64_t fight_id)
{
    Fight& current_fight = findFight(fight_id);

    // For an appropriate handling of the client pokemon's state
    current_fight.handleClientStats();

    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    current_fight.decreaseServerHPDueToPunch();

    if(isFightStopped(_return, current_fight, fight_id))
    {
        return;
    }

    serverAction(current_fight, _return);

    if(isFightStopped(_return, current_fight, fight_id))
    {
        return;
    }

    // TODO Добавить возврат строки с описание произошедшего за раунд. Мб удалить это вообще?
    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
}

void PokServerHandler::defend(RoundResult& _return, const int64_t fight_id)
{
    Fight& current_fight = findFight(fight_id);

    // For an appropriate handling of the client pokemon's state
    current_fight.handleClientStats();

    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    current_fight.setClientDefense();

    serverAction(current_fight, _return);

    if(isFightStopped(_return, current_fight, fight_id))
    {
        return;
    }

    // TODO Добавить возврат строки с описание произешедшего за раунд
    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
}

void PokServerHandler::useSkill(RoundResult& _return, const int64_t fight_id, const std::string& skillName)
{
    Fight& current_fight = findFight(fight_id);

    // For an appropriate handling of the client pokemon's state
    current_fight.handleClientStats();

    // Extract pokemons from fight object
    auto& c_pok = current_fight.getClientPok();
    auto& s_pok = current_fight.getServerPok();

    const PokemonSkill& c_skill = c_pok.skill;
    if(!c_skill.amount)
    {
        _return.__set_actionResultDescription("Your pokemon cunt use skills anymore!\n"
                                               "You`ve just screwed up your turn ┌∩┐(◣_◢)┌∩┐");
        _return.__set_clientPokemon(c_pok);
        _return.__set_serverPokemon(s_pok);
    }
    else
    {
        // TODO Не забыть в кейзах прописать нарезку результирующей строки с описанием раунда
        // Depends on skill type
        switch(c_skill.type)
        {
            case SkillType::BUFF:
                // Buff client pok
                current_fight.setClientBuf();
                break;
            case SkillType::DEBUFF:
                // Debuff server pok
                current_fight.setServerDebuf();
                break;
            case SkillType::ATTACK:
                // Decrease server pok HP
                current_fight.decreaseServerHPDueToSkill();

                if(isFightStopped(_return, current_fight, fight_id))
                {
                    return;
                }
                break;
        }

    }

    serverAction(current_fight, _return);

    if(isFightStopped(_return, current_fight, fight_id))
    {
        return;
    }

    _return.__set_clientPokemon(c_pok);
    _return.__set_serverPokemon(s_pok);
}
