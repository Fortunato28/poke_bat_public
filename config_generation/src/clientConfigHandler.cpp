#include "clientConfigHandler.h"

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
    std::ofstream fout("config.cfg", std::ios_base::trunc);
    fout << configContent;
    fout.close(); 
}

void clientConfigHandler::load_config_from_file()
{
    FILE * ptrFile = fopen("config.cfg", "rb");

    if (ptrFile ==  NULL)
    {
        printf("Can't open file!\n");
        exit(1);
    }

    fseek(ptrFile, 0, SEEK_END);
    long iSize = ftell(ptrFile);
    rewind(ptrFile);
    char* buffer = (char*) malloc(sizeof(char) * iSize);
    size_t result = fread(buffer, 1, iSize, ptrFile);

    std::string temp(buffer, result);
    configContent = temp;

    fclose(ptrFile);
    free(buffer);
}

void clientConfigHandler::decrypt_config()
{  
    char *bufferContent = new char[configContent.length() + 1];
    for (int i = 0; i < configContent.length(); ++i)
    {
        bufferContent[i] = configContent.c_str()[i];
    }
    bufferContent[configContent.length()] = '\0';

    char password[] = "12345678";

    char salt[] = "12345678";

    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    size_t bufferContentLength = configContent.length();
    char* buffer = (char*)malloc(bufferContentLength);

    gcryError = gcry_cipher_open(&descriptorPointer,
                                 GCRY_CIPHER_DES,
                                 GCRY_CIPHER_MODE_CBC,
                                 GCRY_CIPHER_CBC_CTS);
    if (gcryError) {
        printf("gcry_cipher_open failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_setkey(descriptorPointer, password, 8);
    if (gcryError) {
        printf("gcry_cipher_setkey failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_setiv(descriptorPointer, salt, 8);
    if (gcryError) {
        printf("gcry_cipher_setiv failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcryError = gcry_cipher_decrypt(descriptorPointer, buffer, 
                                    bufferContentLength, bufferContent, bufferContentLength);
    if (gcryError) {
        printf("gcry_cipher_encrypt failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcry_cipher_close(descriptorPointer);

    std::string temp(buffer, bufferContentLength);
    configContent = temp;

    delete [] bufferContent;
    free(buffer);
}
