#include <iostream>
#include <memory>
#include <thread>

//// TODO think about it
// TODO почему тут-то не работает?
//#define host_ "localhost"
//#define port_ 3990

#include <thrift/server/TNonblockingServer.h>

class ServerController
{
public:
    // При создании объекта запускается стартует трифт-сервер
    ServerController();
    ~ServerController();

private:
    // Простите, но это синглтон
    ServerController(const ServerController& other) = delete;
    ServerController(ServerController&& other) = delete;
    ServerController& operator=(const ServerController& other) = delete;
    ServerController& operator=(ServerController&& other) = delete;

    std::shared_ptr<::apache::thrift::server::TNonblockingServer> server_;
    std::thread serverThread_;
};
