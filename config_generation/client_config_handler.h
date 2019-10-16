/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data on the client side. Nothing special, really ¯\_(ツ)_/¯.
*/

#include <fstream>
#include <sstream>
#include <functional>

#include <libconfig.h++>
#include "gen-cpp/interfaces_types.h"

class clientConfigHandler
{
public:
    clientConfigHandler();
    bool IsConfigExist();
    void GetDefaultConfig(std::function<void(std::string&)> callback_to_get_from_server);
    void SaveConfigToFile();
    void LoadConfigFromFile();
    poke_bat::middleware::Pokemon ParseConfig();
    void UpdateConfig(poke_bat::middleware::Pokemon pokemon);
private:
    std::string configContent_;
    std::string configFileName_;
};
