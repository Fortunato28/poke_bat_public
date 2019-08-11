#include "serverConfigHandler.h"

serverConfigHandler::serverConfigHandler()
{
    content = "pokemon:\n"
        "{\n"
            "\tname = \"picachu\";\n" 
            "\ttype = \"electric\";\n" 
            "\tHP = 100;\n"
            "\tAttack = 10;\n"
            "\tDefense = 10;\n"
            "\tSp.Atk = 10;\n"
            "\tSp.Def = 10;\n"
            "\tSpeed = 40;\n"
            "\tEXP = 0;\n"
            "\tLVL = 0;\n"
            "\tskills = ( (\"lightning bow\","
                        "\"atk\","
                        "4," 
                        "\"This bow's gonna light your enemy up.\") );\n"
        "};";

    password = "12345678";
    
    salt = "12345678";
}

void serverConfigHandler::encrypt_config()
{
    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    
    char* contentBuffer = new char[(content.length() + 1) * 8];
    strcpy(contentBuffer, content.c_str());
    size_t contentBufferLength = strlen(contentBuffer);
    
    char* passwordBuffer = new char[password.length() + 1];
    strcpy(passwordBuffer, password.c_str());
     
    char* saltBuffer = new char[salt.length() + 1];
    strcpy(saltBuffer, salt.c_str());
    
    //printf("Text:\n%s\n\n", contentBuffer);

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

    gcryError = gcry_cipher_encrypt(descriptorPointer, contentBuffer, 
                                    contentBufferLength * 8, NULL, 0);
    if (gcryError) {
        printf("gcry_cipher_encrypt failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    gcry_cipher_close(descriptorPointer);
 
    content = std::string(contentBuffer, contentBufferLength);

    //printf("Encrypted text:\n%s\n\n", contentBuffer);
    
    delete [] contentBuffer;
    delete [] passwordBuffer;
    delete [] saltBuffer;
}

