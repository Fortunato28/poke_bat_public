#include <iostream>
#include <memory>

#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include "gen-cpp/PokServer.h"

#include "client_controller.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace poke_bat::middleware;

#define host_ "localhost"
#define port_ 3990

ClientController::ClientController()
{
    shared_ptr<TSocket> socket(new TSocket(host_, port_));
    shared_ptr<TTransport> transport(new TFramedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    PokServerClient client(protocol);

    try
    {
        transport->open();

        // TODO почему конфиг так легко подключился?
        Config test;
        client.getConfig(test);

        transport->close();
    }

    catch (TException& tx)
    {
        cout << "ERROR: " << tx.what() << endl;
    }
    printf("HERE %s\n", "Client constructor WORKED!");
}
