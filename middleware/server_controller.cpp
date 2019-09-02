#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TNonblockingServerSocket.h>

#include "gen-cpp/PokServer.h"
#include "server_controller.h"
#include "pok_server.h"

// TODO think about it
#define host_ "localhost"
#define port_ 3990

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace poke_bat::middleware;

ServerController::ServerController()
{
    std::shared_ptr<PokServerHandler> handler(new PokServerHandler());
    std::shared_ptr<TProcessor> processor(new PokServerProcessor(handler));
    std::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
    std::shared_ptr<TNonblockingServerTransport> serverTransport(new TNonblockingServerSocket(host_, port_));
    std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    std::shared_ptr<PosixThreadFactory> threadFactory = std::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    printf("HERE %s\n", "Constructor started!");
}
