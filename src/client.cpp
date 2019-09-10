#include <iostream>
#include "server_controller.h"
#include "client_controller.h"

int main()
{
    // TODO здесь запуск клиента клиента
    ClientController client;
    Config config;
    client.getConfig(config);

    return 0;
}
