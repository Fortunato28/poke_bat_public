#include "fight.h"

Fight::Fight(Pokemon client_pokemon, Pokemon server_pokemon) :
    clientState_(client_pokemon),
    serverState_(server_pokemon)
{
}

// По этой формуле высчитываются все изменения статов покемонов
int64_t useSuperFormula(int64_t lvl, int64_t attack, int64_t def)
{
    return ((((2 * lvl / 5) + 2) * (attack * attack / def) / 50) + 2);
}

Pokemon& Fight::getClientPok()
{
    return clientState_.pokemon_;
}

Pokemon& Fight::getServerPok()
{
    return serverState_.pokemon_;
}

void Fight::decreaseClientHPDueToPunch()
{
    auto& s_pok = serverState_.pokemon_;
    auto& c_pok = clientState_.pokemon_;

    c_pok.HP -= useSuperFormula(s_pok.LVL, s_pok.attack, c_pok.defense);
}

void Fight::decreaseClientHPDueToSkill()
{
    // TODO проверить корректность
    auto& s_pok = serverState_.pokemon_;
    auto& c_pok = clientState_.pokemon_;
    --s_pok.skill.amount;

    c_pok.HP -= useSuperFormula(s_pok.LVL, s_pok.spell_attack, c_pok.spell_defense);
}

void Fight::decreaseServerHPDueToPunch()
{
    auto& s_pok = serverState_.pokemon_;
    auto& c_pok = clientState_.pokemon_;

    s_pok.HP -= useSuperFormula(c_pok.LVL, c_pok.attack, s_pok.defense);
}

void Fight::decreaseServerHPDueToSkill()
{
    auto& s_pok = serverState_.pokemon_;
    auto& c_pok = clientState_.pokemon_;
    --c_pok.skill.amount;

    s_pok.HP -= useSuperFormula(c_pok.LVL, c_pok.spell_attack, s_pok.spell_defense);
}

void Fight::setClientDefense()
{
    clientState_.setDefense();
}

int64_t calculateSkillDamage(int64_t lvl, int64_t attack, int64_t def)
{
    return useSuperFormula(lvl, attack, def);
}

int64_t calculateBuf(int64_t lvl, int64_t attack, int64_t def)
{
    return useSuperFormula(lvl, attack, def);
}

void Fight::setServerDefense()
{
    serverState_.setDefense();
}

void Fight::setClientBuf()
{
    auto& c_pok = clientState_.pokemon_;
    --c_pok.skill.amount;

    if(clientState_.bufRoundCounter_ > 0)
    {
        clientState_.bufRoundCounter_ = 3;
        return;
    }
    else
    {
        auto bufSize = calculateBuf(c_pok.LVL, c_pok.spell_attack, c_pok.spell_defense);
        clientState_.buf_ += bufSize;
        c_pok.attack += clientState_.buf_;
        clientState_.bufRoundCounter_ = 3;
    }
}

void Fight::setServerDebuf()
{
    auto& c_pok = clientState_.pokemon_;
    --c_pok.skill.amount;

    if(serverState_.debufRoundCounter_ > 0)
    {
        serverState_.debufRoundCounter_ = 3;
        return;
    }
    else
    {
        auto& s_pok = serverState_.pokemon_;

        auto debufSize = calculateBuf(c_pok.LVL, c_pok.spell_attack, s_pok.spell_defense);
        serverState_.debuf_ += debufSize;

        s_pok.attack -= serverState_.debuf_;

        // Если дебафнули слишком сильно, пусть урон будет минимальным
        if (s_pok.attack < 0)
        {
            s_pok.attack = 0;
        }
        serverState_.debufRoundCounter_ = 3;
    }
}

void Fight::setServerBuf()
{
    auto& s_pok = serverState_.pokemon_;
    --s_pok.skill.amount;

    if(serverState_.bufRoundCounter_ > 0)
    {
        serverState_.bufRoundCounter_ = 3;
        return;
    }
    else
    {
        auto bufSize = calculateBuf(s_pok.LVL, s_pok.spell_attack, s_pok.spell_defense);
        serverState_.buf_ += bufSize;
        s_pok.attack += serverState_.buf_;
        serverState_.bufRoundCounter_ = 3;
    }
}

void Fight::setClientDebuf()
{
    auto& s_pok = serverState_.pokemon_;
    --s_pok.skill.amount;

    if(clientState_.debufRoundCounter_ > 0)
    {
        clientState_.debufRoundCounter_ = 3;
        return;
    }
    else
    {
        auto& c_pok = clientState_.pokemon_;

        auto debufSize = calculateBuf(s_pok.LVL, s_pok.spell_attack, c_pok.spell_defense);
        clientState_.debuf_ += debufSize;

        c_pok.attack -= clientState_.debuf_;

        // Если дебафнули слишком сильно, пусть урон будет минимальным
        if (c_pok.attack < 0)
        {
            c_pok.attack = 0;
        }
        clientState_.debufRoundCounter_ = 3;
    }

}

void Fight::handleClientStats()
{
    // Скоуп, в котором контролируется длительность бафа
    {
        // Баф кончился
        if(!clientState_.bufRoundCounter_)
        {
            clientState_.pokemon_.attack -= clientState_.buf_;
            clientState_.buf_ = 0;
        }

        // Если баф есть, то уменьшаем его длительность
        --clientState_.bufRoundCounter_;

    }

    // Скоуп, в котором контролируется длительность дебафа
    {
        // Дебаф кончился
        if(!clientState_.debufRoundCounter_)
        {
            clientState_.pokemon_.attack = clientState_.defaultAttack_;
            clientState_.debuf_ = 0;
        }

        // Если баф есть, то уменьшаем его длительность
        --clientState_.debufRoundCounter_;

    }

    if(clientState_.isInDefense_)
    {
        clientState_.isInDefense_ = false;
        clientState_.pokemon_.defense /= 2;
    }
}

void Fight::handleServerStats()
{
    // Скоуп, в котором контролируется длительность бафа
    {
        // Баф кончился
        if(!serverState_.bufRoundCounter_)
        {
            serverState_.pokemon_.attack -= serverState_.buf_;
            serverState_.buf_ = 0;
        }

        // Если баф есть, то уменьшаем его длительность
        --serverState_.bufRoundCounter_;

    }
    // Скоуп, в котором контролируется длительность дебафа
    {
        // Дебаф кончился
        if(!serverState_.debufRoundCounter_)
        {
            if(!serverState_.defaultAttack_)
            {
                serverState_.pokemon_.attack += serverState_.debuf_;
            }
            else
            {
                serverState_.pokemon_.attack = serverState_.defaultAttack_;
            }
            serverState_.debuf_ = 0;
        }

        // Если дебаф есть, то уменьшаем его длительность
        --serverState_.debufRoundCounter_;
    }
    if(serverState_.isInDefense_)
    {
        serverState_.isInDefense_ = false;
        serverState_.pokemon_.defense /= 2;
    }
}

Fight::PokemonState::PokemonState(Pokemon pok):
    pokemon_(pok),
    isInDefense_(false),
    buf_(0),
    bufRoundCounter_(0),
    debuf_(0),
    debufRoundCounter_(0),
    defaultAttack_(pok.attack)
{}

void Fight::PokemonState::setDefense()
{
    isInDefense_ = true;
    pokemon_.defense *= 2;
}
