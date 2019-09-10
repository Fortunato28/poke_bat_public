#pragma once

#include <iostream>
#include <memory>
#include <thread>

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
    const size_t threadsAmount_ = 15;
};
