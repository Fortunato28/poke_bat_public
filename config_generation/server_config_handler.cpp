#include "server_config_handler.h"

void serverConfigHandler::EncryptConfig()
{
    char content[] = "pokemon:\n"
        "{\n"
            "\tname = \"picachu\";\n"
            "\ttype = \"electric\";\n"
            "\tHP = 100;\n"
            "\tAttack = 10;\n"
            "\tDefense = 10;\n"
            "\tSp_Atk = 10;\n"
            "\tSp_Def = 10;\n"
            "\tSpeed = 40;\n"
            "\tEXP = 0;\n"
            "\tLVL = 0;\n"
            "\tskills = ( (\"lightning bow\","
                        "\"atk\","
                        "4) );\n"
        "};";
    
    char password[] = "12345678";

    char salt[] = "12345678";

    gcry_error_t        gcryError;
    gcry_cipher_hd_t    descriptorPointer;
    size_t contentLength = strlen(content);
    size_t passwordLength = strlen(password);
    size_t saltLength = strlen(salt);
    char* buffer = (char*)malloc(contentLength);

    //crypto-descryptor initialisation
    gcryError = gcry_cipher_open(&descriptorPointer,
                                 GCRY_CIPHER_DES,
                                 GCRY_CIPHER_MODE_CBC,
                                 GCRY_CIPHER_CBC_CTS);
    if (gcryError) {
        printf("gcry_cipher_open failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //set key for the ecryption
    gcryError = gcry_cipher_setkey(descriptorPointer, password, passwordLength);
    if (gcryError) {
        printf("gcry_cipher_setkey failed:  %s/%s\n", 
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //set salt for the encryption
    gcryError = gcry_cipher_setiv(descriptorPointer, salt, saltLength);
    if (gcryError) {
        printf("gcry_cipher_setiv failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //encryption
    gcryError = gcry_cipher_encrypt(descriptorPointer, 
                                    buffer, 
                                    contentLength, 
                                    content, 
                                    contentLength);
    if (gcryError) {
        printf("gcry_cipher_encrypt failed:  %s/%s\n", 
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return;
    }

    //closing crypto-descryptor
    gcry_cipher_close(descriptorPointer);
    
    //conversion encrypted data to std::string
    std::string temp(buffer, contentLength);
    encryptedContent = temp;
    
    free(buffer);
}
