/* 
 * This header contain the class which will encrypt and store pokemon data. 
 * Nothing special, really ¯\_(ツ)_/¯.
*/

#include <gcrypt.h>
#include <stdio.h>
#include <string>

class serverConfigHandler
{
public:
    void EncryptConfig();
private:
    std::string encryptedContent;
};
