namespace cpp poke_bat.middleware

enum PokemonType {
    NORMAL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    ICE,
    FIGHTING,
    POISON,
    GROUND,
    FLYING,
    PSYCHIC,
    BUG,
    ROCK,
    GHOST,
    DARK,
    DRAGON,
    STEEL,
    FAIRY
}

enum SkillType {
    ATTACK,
    BUFF,
    DEBUFF
}

struct PokemonSkill {
    1: required string name;
    2: required SkillType type;
    4: required i64 amount;
}

struct Pokemon {
    1: required string name;
    2: required PokemonType type;
    3: required i64 HP;
    4: required i64 attack;
    5: required i64 defense;
    6: optional i64 spell_attack;
    7: optional i64 spell_defense;
    8: optional i64 speed;
    9: required i64 EXP;
    10: required i64 LVL;
    11: required PokemonSkill skill;
    12: required string flag;
}

struct RoundResult {
    1: required Pokemon clientPokemon;
    2: required Pokemon serverPokemon;
    3: required string actionResultDescription;
}

struct FightData {
    1: required Pokemon pokemon;
    2: required i64 fight_id;
}

service PokServer {
    string getConfig(),

    FightData startFight(1:i64 complexity, 2:Pokemon clientPokemon)
    RoundResult punch(1:i64 fight_id),
    RoundResult defend(1:i64 fight_id),
    RoundResult useSkill(1:i64 fight_id, 2:string skillName)

    string savePokemon(1:string private_id, 2:Pokemon client_pokemon, 3:string comment)
    string getSavedPoksTable()
}

