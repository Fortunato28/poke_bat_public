cmake_minimum_required(VERSION 3.7)

find_library(LibGCrypt 
    NAMES libgcrypt.so libgcrypt.a
    PATHS "/usr/lib/x86_64-linux-gnu")

find_library(LibGCrypt 
    NAMES libconfig.so libconfig.a
    PATHS "/usr/lib/x86_64-linux-gnu")

list(APPEND HEADERS "src")

list(APPEND SOURCES "src/clientConfigHandler.h")
list(APPEND SOURCES "src/serverConfigHandler.h")
list(APPEND SOURCES "src/clientConfigHandler.cpp")
list(APPEND SOURCES "src/serverConfigHandler.cpp")
list(APPEND SOURCES "main.cpp")

include_directories(${HEADERS})

add_executable(test_config_generation ${SOURCES})

target_link_libraries(test_config_generation ${LibGCrypt})
