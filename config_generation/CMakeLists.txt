cmake_minimum_required(VERSION 3.7)

find_library(LibGCrypt 
    NAMES libgcrypt.so libgcrypt.a
    PATHS "/usr/lib/x86_64-linux-gnu")

find_library(LibConfig 
    NAMES libconfig++.so libconfig++.a
    PATHS "/usr/lib/x86_64-linux-gnu")

include_directories(../middleware
     /usr/local/include
     /usr/local/include/cppconn
     ${CMAKE_BINARY_DIR}
     /usr/include
     /usr/include/mysql
     /usr/include/mysqlx
     /usr/include/jdbc
     )

 set(CONFIG_HANDLER_SOURCES client_config_handler.cpp
                            server_config_handler.cpp
                            )
add_library(config_handler ${CONFIG_HANDLER_SOURCES})
target_link_libraries(config_handler middleware)

add_executable(test_config_generation main.cpp)

target_link_libraries(test_config_generation config_handler)
target_link_libraries(test_config_generation ${LibGCrypt})
target_link_libraries(test_config_generation ${LibConfig})
