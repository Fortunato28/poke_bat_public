/* 
 * This header contain the class which will handle basic config file 
 * with pokemon data on the server side. Nothing special, really ¯\_(ツ)_/¯.
 * (I will fix this description later during class development.)
*/
#include <gcrypt.h>
#include <stdio.h>
#include <string>

class serverConfigHandler
{
public:
    serverConfigHandler() : encryptedContent("") {}
    void encrypt_config();
    void send_default_config();
private:
    std::string encryptedContent;
};
