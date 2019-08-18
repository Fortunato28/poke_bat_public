cmake_minimum_required(VERSION 3.7)

find_library(LibGCrypt 
    NAMES libgcrypt.so libgcrypt.a
    PATHS "/usr/lib/x86_64-linux-gnu")

find_library(LibConfig 
    NAMES libconfig.so libconfig.a
    PATHS "/usr/lib/x86_64-linux-gnu")

#list of dirs with headers
list(APPEND HEADERS_DIRS "src")

#list of headers
list(APPEND HEADERS "src/client_config_handler.h")
list(APPEND HEADERS "src/server_config_handler.h")

#list of sources
list(APPEND SOURCES "src/client_config_handler.cpp")
list(APPEND SOURCES "src/server_config_handler.cpp")
list(APPEND SOURCES "main.cpp")

include_directories(${HEADERS_DIRS})

add_executable(test_config_generation ${HEADERS} ${SOURCES})

target_link_libraries(test_config_generation ${LibGCrypt})
target_link_libraries(test_config_generation ${LibConfig})
