#!/usr/bin/python3
import sys
import glob
import getpass
import os.path
import shutil
from hashlib import sha256
import traceback


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

def parse_pokemon(stat_arr):
    # pokemon type
    pok_t = stat_arr[1].upper().replace('\n', '')
    PT = PokemonType()._NAMES_TO_VALUES[pok_t]

    # skill name
    i1 = 0
    i2 = stat_arr[8].find('_')
    s_name = stat_arr[8][i1:i2]

    # skill type
    i1 = i2 + 1
    i2 = stat_arr[8].find('_', i1)
    s_type = stat_arr[8][i1:i2]
    s_type = SkillType()._NAMES_TO_VALUES[s_type]

    # skill amount
    i1 = i2 + 1
    s_amount = int(stat_arr[8][i1:])

    PS = PokemonSkill(s_name, s_type, s_amount)

    pok = Pokemon(
            stat_arr[0].replace('\n', ''),                #name
            PT,                         #type
            int(stat_arr[2]),           #HP
            int(stat_arr[3]),           #attack
            int(stat_arr[4]),           #defense
            int(stat_arr[5]),           #sp_atk
            int(stat_arr[6]),           #sp_def
            int(stat_arr[7]),           #speed
            0,                          #EXP
            5,                          #LVL
            PS,                         #skill
            '',                         #flag
            '',                         #pub_id
            )

    return pok

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

def put_flag():
    global host, port, f_id, flag, filename
    # try put
    try:
        # print("try connect " + host + ":" + str(port))
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        stat_arr = []
        with open(filename) as f:
            for stat in f.readlines():
                stat_arr.append(stat)

        # # print(content)
        # for line in stat_arr[0:9]:
            # print(line)

        with open(filename, 'w') as f:
            for line in stat_arr[10:]:
                f.write(line)
            for line in stat_arr[0:10]:
                f.write(line)

        pok = parse_pokemon(stat_arr)

        s = client.savePokemon(f_id, pok, flag)
        print(s)

        transport.close()
    except Thrift.TApplicationException as e:
        print(e)
        service_corrupt()
    except Thrift.TException as e:
        print(e)
        service_down()
    except Exception as e:
        # traceback.print_exc()
        print(e)
        # raise e
        service_down()

def check_flag():
    global host, port, f_id, flag
    # try get
    flag2 = ""
    try:
        # print("try connect " + host + ":" + str(port))
        transport = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = Client(protocol)
        transport.open()

        s = client.getSavedPokByPrivateID(f_id)
        flag2 = s.flag
        print(flag2)

        transport.close()
    except Thrift.TApplicationException as e:
        print(e)
        service_corrupt()
    except Thrift.TException as e:
        print(e)
        service_down()
    except Exception as e:
        print(e)
        service_down()

    if flag != flag2:
        service_corrupt()


if command == "put":
    put_flag()
    check_flag()
    service_up()

if command == "check":
    check_flag()
    service_up()
