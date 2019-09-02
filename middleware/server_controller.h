#include <iostream>

class ServerController
{
public:
    // При создании объекта запускается стартует трифт-сервер
    ServerController();

private:
    // Простите, но это синглтон
    ServerController(const ServerController& other) = delete;
    ServerController(ServerController&& other) = delete;
    ServerController& operator=(const ServerController& other) = delete;
    ServerController& operator=(ServerController&& other) = delete;

};
