#include "configHandler.h"

configHandler::configHandler()
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
                        "10, 4," 
                        "\"This bow's gonna light your enemy up.\") );\n"
        "};";
}

void configHandler::encrypt_config()
{
    //TODO find out the way to encrypt the content and implement it
}

std::string configHandler::get_default_config()
{
    return content;
}

void configHandler::decrypt_config()
{
    //TODO same here
}
