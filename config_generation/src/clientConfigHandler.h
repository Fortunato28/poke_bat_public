/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data on the client side. Nothing special, really ¯\_(ツ)_/¯.
 * (I will fix this description later during class development.)
*/
#include <libconfig.h++>
#include <gcrypt.h>
#include <string>
#include <stdio.h>
#include <dirent.h>

class clientConfigHandler
{
public:
    clientConfigHandler();
    bool isConfigExist();
    void get_default_config();
    void save_config_to_file();
    void load_config_from_file(); 
    void decrypt_config();
private:
    std::string content;
    std::string password;
    std::string salt;
};
