#include "server_config_handler.h"

std::string serverConfigHandler::EncryptConfig()
{
    char content[] = "pokemon:\n"
        "{\n"
            "\tname = \"picachu\";\n"
            "\ttype = \"electric\";\n"
            "\tHP = 100;\n"
            "\tAttack = 10;\n"
            "\tDefense = 10;\n"
            "\tSp_Atk = 30;\n"
            "\tSp_Def = 30;\n"
            "\tSpeed = 40;\n"
            "\tEXP = 0;\n"
            "\tLVL = 1;\n"
            "\tskills = ( (\"lightning bow\","
                        "\"attack\","
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
        return {"Bad crypto-descryptor installation"};
    }

    //set key for the ecryption
    gcryError = gcry_cipher_setkey(descriptorPointer, password, passwordLength);
    if (gcryError) {
        printf("gcry_cipher_setkey failed:  %s/%s\n",
                gcry_strsource(gcryError), gcry_strerror(gcryError));
        return {"Bad key for the encryption"};
    }

    //set salt for the encryption
    gcryError = gcry_cipher_setiv(descriptorPointer, salt, saltLength);
    if (gcryError) {
        printf("gcry_cipher_setiv failed:  %s/%s\n",
               gcry_strsource(gcryError), gcry_strerror(gcryError));
        return {"Bad salt for the encryption"};
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
        return {"Bad encryption, fuck off"};
    }

    //closing crypto-descryptor
    gcry_cipher_close(descriptorPointer);
    
    //conversion encrypted data to std::string
    std::string temp(buffer, contentLength);
    encryptedContent = temp;
    
    free(buffer);

    return encryptedContent;
}

