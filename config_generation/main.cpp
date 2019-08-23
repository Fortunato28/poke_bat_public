#include "client_config_handler.h"
#include "server_config_handler.h"

int main()
{
    serverConfigHandler server;
    server.EncryptConfig();
    clientConfigHandler client;
    if (client.IsConfigExist())
    {
        client.LoadConfigFromFile();
        client.DecryptConfig();
        client.ParseConfig();
    }
    //printf("I hate my life!\n");
    return 0;
}
