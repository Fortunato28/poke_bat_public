#!/usr/bin/python3
import sys
import glob
import getpass
import os.path
import shutil
from hashlib import sha256
import traceback
import re

sys.path.append('gen-py')
if (os.path.exists('/usr/local/lib/python3.7/dist-packages/')):
    sys.path.insert(0,
        glob.glob('/usr/local/lib/python3.7/dist-packages/')[0])
if (os.path.exists('/home/' + getpass.getuser() + '/.local/lib/python3.7/site-packages/')):
    sys.path.insert(0,
        glob.glob('/home/' + getpass.getuser() + '/.local/lib/python3.7/site-packages/')[0])

from interfaces.PokServer import Client
from interfaces.ttypes import PokemonType, SkillType, PokemonSkill, Pokemon, RoundResult, FightData

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

#*******************************************************#
# !1 BLOCK WITH OUR SUBFUNCTIONS
#*******************************************************#

def parse_pokemon(stat_arr):
    # pokemon type
    pok_t = stat_arr[1].upper().replace('\n', '')
    pokemon_type = PokemonType()._NAMES_TO_VALUES[pok_t]

    #TODO probably it would be usefull to parse it with 're', but I'm not sure
    # skill name
    end_of_the_slice = stat_arr[8].find('_')
    skill_name = stat_arr[8][:end_of_the_slice]

    # skill type
    begin_of_the_slice = end_of_the_slice + 1
    end_of_the_slice = stat_arr[8].find('_', begin_of_the_slice)
    skill_type = stat_arr[8][begin_of_the_slice:end_of_the_slice]
    skill_type = SkillType()._NAMES_TO_VALUES[skill_type]

    # skill amount
    begin_of_the_slice = end_of_the_slice + 1
    skill_amount = int(stat_arr[8][begin_of_the_slice:])

    pokemon_skill = PokemonSkill(skill_name,
                                 skill_type,
                                 skill_amount)

    pok = Pokemon(
            stat_arr[0].replace('\n', ''),  #name
            pokemon_type,                   #type
            int(stat_arr[2]),               #HP
            int(stat_arr[3]),               #attack
            int(stat_arr[4]),               #defense
            int(stat_arr[5]),               #sp_atk
            int(stat_arr[6]),               #sp_def
            int(stat_arr[7]),               #speed
            0,                              #EXP
            5,                              #LVL
            pokemon_skill,                  #skill
            '',                             #flag
            '',                             #pub_id
            )

    return pok

def log_it_mafacka(output):
    with open("./poke_bat_logs.txt", "a") as f:
        f.write(str(output) + "\n")

#*******************************************************#
# !2 RETURN CODES
#*******************************************************#

# put-get flag to service success
def service_up():
    print("[service is worked] - 101")
    exit(101)

# service is available (available tcp connect) but protocol wrong could not put/get flag
def service_corrupt():
    print("[service is corrupt] - 102")
    exit(102)

# waited time (for example: 5 sec) but service did not have time to reply
def service_mumble():
    print("[service is mumble] - 103")
    exit(103)

# service is not available (maybe blocked port or service is down)
def service_down():
    print("[service is down] - 104")
    exit(104)

#*******************************************************#
# !3 INITIAL STEPS
#*******************************************************#

if len(sys.argv) != 5:
    print("\nUsage:\n\t" + sys.argv[0] + " <host> (put|check) <flag_id> <flag>\n")
    print("Example:\n\t" + sys.argv[0] + " \"127.0.0.1\" put \"abcdifghr\" \"123e4567-e89b-12d3-a456-426655440000\" \n")
    print("\n")
    exit(0)

host = sys.argv[1]
port = 3990
command = sys.argv[2]
f_id = sha256(sys.argv[3].encode('utf-8')).hexdigest()
flag = sys.argv[4]
filename = sys.argv[1] + '.cvg'
if (not os.path.exists(filename)):
    shutil.copy('result.cvg_temp', filename)

# will be mumble (2) - for test jury
# while True: time.sleep(10);

#*******************************************************#
# !4 MAIN CHECKER FUNCTIONS
#*******************************************************#

def put_flag():
    global host, port, f_id, flag, filename
    # try put
    try:
        # print("try connect " + host + ":" + str(port))
        stat_arr = []
        with open(filename) as f:
            for stat in f.readlines():
                stat_arr.append(stat)

        # rewrite current pokemon to the end of file
        with open(filename, 'w') as f:
            for line in stat_arr[10:]:
                f.write(line)
            for line in stat_arr[0:10]:
                f.write(line)

        pok = parse_pokemon(stat_arr)

        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        log_output = client.savePokemon(f_id, pok, flag)
        log_it_mafacka(log_output)

        transport.close()
    except Thrift.TApplicationException as e:
        log_it_mafacka(e)
        service_corrupt()
    except Thrift.TException as e:
        log_it_mafacka(e)
        service_down()
    except Exception as e:
        # traceback.print_exc()
        log_it_mafacka(e)
        # raise e
        service_down()

def check_flag():
    global host, port, f_id, flag
    # try get
    gotten_flag = ""
    try:
        # print("try connect " + host + ":" + str(port))
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        saved_pokemon = client.getSavedPokByPrivateID(f_id)
        gotten_flag = saved_pokemon.flag
        print(saved_pokemon)

        transport.close()
    except Thrift.TApplicationException as e:
        log_it_mafacka(e)
        service_corrupt()
    except Thrift.TException as e:
        log_it_mafacka(e)
        service_down()
    except Exception as e:
        log_it_mafacka(e)
        service_down()

    if flag != gotten_flag:
        service_corrupt()

def is_saved_pokemon_exist_in_db():
    global host, port, f_id, flag
    try:
        pok = Pokemon(
            "picachu",                                          #name
            PokemonType.NORMAL,                                 #type
            10,                                                 #HP
            11,                                                 #attack
            12,                                                 #defense
            13,                                                 #sp_atk
            14,                                                 #sp_def
            15,                                                 #speed
            0,                                                  #EXP
            2,                                                  #LVL
            PokemonSkill("lightning", SkillType.ATTACK, 3),     #skill
            '',                                                 #flag
            '',                                                 #pub_id
            )

        # Thrifts shit
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        # Save pokemon and get its pub_id
        saved_result = client.savePokemon("zhupa", pok, "test_checker")
        base_for_search = "with pub_id = "
        index_for_search_pub_id = saved_result.find(base_for_search)
        saved_pok_pub_id = saved_result[index_for_search_pub_id + len(base_for_search):].replace('\n', '')

        # Get table and pud_id there
        table = client.getSavedPoksTable()
        transport.close()
        pub_id_index_in_table = table.find(" " + saved_pok_pub_id + " ")
        pub_id_in_table = table[pub_id_index_in_table + 1:pub_id_index_in_table + len(saved_pok_pub_id) + 1]

        # TODO test! Strange things
        #pub_id_in_table = "konb"

        if pub_id_in_table != saved_pok_pub_id:
            log_it_mafacka(pub_id_in_table + " " + saved_pok_pub_id)
            service_corrupt()

        # Get string with certain pokemon
        spliter_index = table.find("|\n", pub_id_index_in_table)
        row_with_pok = table[pub_id_index_in_table:spliter_index + 1]

        # TODO test!
        # Check correctness of saved in table pokemons
        if row_with_pok.find(pok.name) == -1:
            service_corrupt()

        if row_with_pok.find(PokemonType()._VALUES_TO_NAMES[pok.type]) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.HP)) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.attack)) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.defense)) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.spell_attack)) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.spell_defense)) == -1:
            service_corrupt()

        if row_with_pok.find(str(pok.LVL)) == -1:
            service_corrupt()
    except Thrift.TApplicationException as e:
        log_it_mafacka(e)
        service_corrupt()
    except Thrift.TException as e:
        log_it_mafacka(e)
        service_down()
    except Exception as e:
        log_it_mafacka(e)
        service_down()

def is_pok_lvl_upped():
    global host, port, f_id, flag
    try:
        # Thrifts shit
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        # Get config with pokemon
        gotten_conf = client.getConfig()
        transport.close()

        test_skill_name = re.findall(r'skills = \( \("(.+?)",', gotten_conf)[0]
        test_skill_type = re.findall(r'\",\s*\"([A-Z]+)\",', gotten_conf)[0]
        test_skill_amount = re.findall(r'skills = .*",(\d)\)\s*\)', gotten_conf)[0]
        test_skill = PokemonSkill(test_skill_name,
                                  SkillType()._NAMES_TO_VALUES[test_skill_type],
                                  int(test_skill_amount))

        test_name = re.findall(r'name = "(\w+)"', gotten_conf)[0]
        test_type = PokemonType()._NAMES_TO_VALUES[re.findall(r'type = "([A-Z]+)"', gotten_conf)[0]]
        test_HP = re.findall(r'HP = ([1-9]+)', gotten_conf)[0]
        test_Attack = re.findall(r'Attack = ([1-9]+)', gotten_conf)[0]
        test_Defense = re.findall(r'Defense = ([1-9]+)', gotten_conf)[0]
        test_Sp_Atk = re.findall(r'Sp_Atk = ([1-9]+)', gotten_conf)[0]
        test_Sp_Def = re.findall(r'Sp_Def = ([1-9]+)', gotten_conf)[0]
        test_Speed = re.findall(r'Speed = ([1-9]+)', gotten_conf)[0]
        test_EXP = re.findall(r'EXP = (\d+)', gotten_conf)[0]
        test_LVL = re.findall(r'LVL = (\d+)', gotten_conf)[0]
        test_flag = re.findall(r'key = \"(\w+)\"', gotten_conf)[0]

        #TODO figure out the reason of our pokemon's verifications trouble
        # log_it_mafacka(gotten_conf)
        # log_it_mafacka(test_type)

        pok = Pokemon(
                test_name,
                test_type,
                int(test_HP),
                int(test_Attack),
                int(test_Defense),
                int(test_Sp_Atk),
                int(test_Sp_Def),
                int(test_Speed),
                int(test_EXP),
                int(test_LVL),
                test_skill,
                test_flag,
                ''
                )

        # Save weak pokemon
        #TODO move it to the subfunctions
        weak_pok = Pokemon(
            "picachu",                                          #name
            PokemonType.NORMAL,                                 #type
            10,                                                 #HP
            11,                                                 #attack
            12,                                                 #defense
            13,                                                 #sp_atk
            14,                                                 #sp_def
            15,                                                 #speed
            0,                                                  #EXP
            2,                                                  #LVL
            PokemonSkill("lightning", SkillType.ATTACK, 3),     #skill
            '',                                                 #flag
            '',                                                 #pub_id
            )

        # Save pokemon and get its pub_id
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        saved_result = client.savePokemon("zhupa", weak_pok, "test_checker")
        base_for_search = "with pub_id = "
        index_for_search_pub_id = saved_result.find(base_for_search)
        saved_pok_pub_id = saved_result[index_for_search_pub_id + len(base_for_search):].replace('\n', '')

        # Handle Fucking fight
        answer = client.startFight(saved_pok_pub_id, pok)
        # log_it_mafacka(answer)
        #TODO get fight_id
        # client.punch(fight_id)

        transport.close()
    except Thrift.TApplicationException as e:
        log_it_mafacka(e)
        service_corrupt()
    except Thrift.TException as e:
        log_it_mafacka(e)
        service_down()
    except Exception as e:
        log_it_mafacka(e)
        service_down()

#*******************************************************#
# !5 MAIN TEST CODE
#*******************************************************#

if command == "put":
    put_flag()
    check_flag()
    #is_saved_pokemon_exist_in_db()
    is_pok_lvl_upped()
    service_up()

if command == "check":
    check_flag()
    service_up()
