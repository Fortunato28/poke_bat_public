/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data on the client side. Nothing special, really ¯\_(ツ)_/¯.
*/

#include <fstream>
#include <map>
#include <functional>

#include <libconfig.h++>
#include <gcrypt.h>
#include <dirent.h>

#include "gen-cpp/interfaces_types.h"

class clientConfigHandler
{
public:
    bool IsConfigExist();
    void GetDefaultConfig(std::function<void(std::string&)> callback_to_get_from_server);
    void SaveConfigToFile();
    void LoadConfigFromFile(); 
    void DecryptConfig();
    poke_bat::middleware::Pokemon ParseConfig();
private:
    std::string configContent_;
};
