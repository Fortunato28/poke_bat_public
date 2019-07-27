/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data, e.g. encrypt it, save it to file and parse. 
 * Nothing special, really ¯\_(ツ)_/¯.
 * (I will fix this description later during class development.)
 *
 * Maybe it's worth to split it between two classes and add one more
 * class with interface only.
*/
#include <libconfig.h++>
#include <iostream>

class configHandler
{
public:
    configHandler();
    //server methods
    void encrypt_config();
    std::string get_default_config();
    //client methods
    void receive_config() {}; //TODO setter?
    void save_config_to_file() {};
    void load_config_from_file() {}; 
    void decrypt_config();
    //TODO add methods for parsing
private:
    std::string content;
};
