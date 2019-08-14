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
    clientConfigHandler() : configContent("") {}
    bool isConfigExist();
    void get_default_config();
    void save_config_to_file();
    void load_config_from_file(); 
    void decrypt_config();
private:
    std::string configContent;
};
