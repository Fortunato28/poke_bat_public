/* 
 * This header contain the class which will encrypt and send pokemon data 
 * to the client. Nothing special, really ¯\_(ツ)_/¯.
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
