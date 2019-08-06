namespace cpp poke_bat.middleware

// Данил, заполни это - зависит от боевой системы и типов покемонов, которые у нас будут
enum PokemonType {
    FIRE,
    ICE,
    ELECTRIC
}

struct PokemonSkill {
    1: required string name;
    2: required PokemonType type; // Всегда ли типы скилов покемонов совпадают с типами самих покемонов? Или нужен отдельный enum для этого поля?
    3: required i64 value; // Что это такое?
    4: required i64 amount;
    5: required string description;
}

struct Pokemon {
    1: required string name;
    2: required PokemonType type;
    3: required i64 HP;
    4: required i64 attack;
    5: required i64 defense;
    6: optional i64 speed_attack;
    7: optional i64 speed_defense;
    8: optional i64 speed;
    9: required i64 EXP;
    10: required i64 LVL;
    11: required PokemonSkill skill;
}

struct RoundResult {
    1: required Pokemon clientPokemon;
    2: required Pokemon serverPokemon;
    3: required string actionResultDescription;
}

struct Config {
    1: required Pokemon pokemon;
    2: required i64 victories;
    3: required i64 defeats;
}

service PokServer {
    Config getConfig(),

    // Методы ниже относятся непосредственно к битве
    Pokemon startFight(1:i64 complexity, 2:Pokemon clientPokemon)
    RoundResult punch(),
    RoundResult defend(),
    RoundResult useSkill(1:string skillName)
}

