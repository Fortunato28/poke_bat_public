#include "clientConfigHandler.h"

clientConfigHandler::clientConfigHandler()
{ 
    password = "12345678";
    
    salt = "12345678";
}

bool clientConfigHandler::isConfigExist()
{
    return true;
}

void clientConfigHandler::get_default_config()
{
    //TODO sending a request for the new config, recieving it and saving
}

void clientConfigHandler::save_config_to_file()
{
    //TODO create file and write content in there
}

void clientConfigHandler::load_config_from_file()
{
    //TODO open file and read the contnent to the variable
}

void clientConfigHandler::decrypt_config()
{ 
    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    
    char* contentBuffer = new char[(content.length() + 1) * 8];
    strcpy(contentBuffer, content.c_str());
    size_t contentBufferLength = strlen(contentBuffer); //SIZE IS IMPORTANT
    
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

    //printf("Decrypted text:\n%s\n\n", contentBuffer);
    
    delete [] contentBuffer;
    delete [] passwordBuffer;
    delete [] saltBuffer;
}
