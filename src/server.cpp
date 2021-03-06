#include <iostream>
#include "server_controller.h"
#include "client_controller.h"
#include <string>
#include "config_parser.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " %file.conf%" << std::endl;
        return -1;
    }

    ConfFileParser cnf((std::string(argv[1])));

    if (!cnf.parseConfig()) {
        std::cout << "Could not parse file: " << argv[1] << std::endl;
        return -2;
    }


    std::string dbhost = cnf.getStringValueFromConfig("mysql.dbhost", "127.0.0.1");
    int dbport = cnf.getIntValueFromConfig("mysql.dbport", 3306);
    std::string dbuser = cnf.getStringValueFromConfig("mysql.dbuser", "user");
    std::string dbpass = cnf.getStringValueFromConfig("mysql.dbpass", "pass");
    std::string dbname = cnf.getStringValueFromConfig("mysql.dbname", "poke_bat");

    // This object has thrift server inside
    ServerController server(dbhost, dbport, dbuser, dbpass, dbname);

    return 0;
}
