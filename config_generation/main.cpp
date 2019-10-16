#include <iostream>
#include <fstream>
#include "server_config_handler.h"
#include "client_config_handler.h"

int main()
{
    serverConfigHandler sch;
    std::ofstream fout;
    fout.open("config.cfg", std::ios_base::trunc);
    fout << sch.SignConfig();
    fout.close();
    clientConfigHandler cch;
    if(cch.IsConfigExist())
    {
        cch.LoadConfigFromFile();
        cch.ParseConfig();
    }
    return 0;
}
