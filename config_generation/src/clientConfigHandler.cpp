#include "clientConfigHandler.h"

clientConfigHandler::clientConfigHandler()
{
    content = "";

    password = "12345678";
    
    salt = "12345678";
}

bool clientConfigHandler::isConfigExist()
{
    int found;
    struct dirent **nameList;
    
    found = scandir(".", &nameList, 0, 0);
    if (found < 0)
    {
        return false;
    }
    else
    {
        while (found--)
        {
            //printf("%s\n", nameList[found]->d_name);
            if ((strncmp(nameList[found]->d_name, "config.cfg", 10)) == 0)
            {
                free(nameList);
                return true;
            }
            free(nameList[found]); 
        }
    }
    free(nameList);
    return false;
}

void clientConfigHandler::get_default_config()
{
    //TODO sending a request for the new config, recieving it and saving
}

void clientConfigHandler::save_config_to_file()
{
    std::ofstream fout("config.cfg");
    fout << content;
    //fout << "test";
    fout.close(); 
}

void clientConfigHandler::load_config_from_file()
{
    std::string tempString;
    std::ifstream fin("config.cfg");
    while (getline(fin, tempString))
    {
        content = content + tempString;
    }
    fin.close();
}

void clientConfigHandler::decrypt_config()
{ 
    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
   
    char* contentBuffer = new char[(content.length() + 1) * 8];
    strcpy(contentBuffer, content.c_str());
    size_t contentBufferLength = strlen(contentBuffer) * 0 + 234; //SIZE IS IMPORTANT
    
    char* passwordBuffer = new char[password.length() + 1];
    strcpy(passwordBuffer, password.c_str());
     
    char* saltBuffer = new char[salt.length() + 1];
    strcpy(saltBuffer, salt.c_str());
    
    gcryError = gcry_cipher_open(&descriptorPointer,
                                 GCRY_CIPHER_DES,
                                 GCRY_CIPHER_MODE_CBC,
                                 GCRY_CIPHER_SECURE);
    if (gcryError) {
        printf("gcry_cipher_open failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_setkey(descriptorPointer, passwordBuffer, 8);
    if (gcryError) {
        printf("gcry_cipher_setkey failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_setiv(descriptorPointer, saltBuffer, 8);
    if (gcryError) {
       printf("gcry_cipher_setiv failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_decrypt(descriptorPointer, contentBuffer, 
                                    contentBufferLength * 8, NULL, 0);
    if (gcryError) {
       printf("gcry_cipher_decrypt failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcry_cipher_close(descriptorPointer);

    content = std::string(contentBuffer, contentBufferLength);

    printf("Decrypted text:\n%s\n\n", contentBuffer);
    
    delete [] contentBuffer;
    delete [] passwordBuffer;
    delete [] saltBuffer;
}
