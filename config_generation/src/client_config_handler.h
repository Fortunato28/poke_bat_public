/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data on the client side. Nothing special, really ¯\_(ツ)_/¯.
*/

#include <libconfig.h++>
#include <gcrypt.h>
#include <dirent.h>
#include <fstream>

class clientConfigHandler
{
public:
    bool IsConfigExist();
    void GetDefaultConfig();
    void SaveConfigToFile();
    void LoadConfigFromFile(); 
    void DecryptConfig();
    void ParseConfig();
private:
    std::string configContent;
};
