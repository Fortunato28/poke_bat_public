cmake_minimum_required(VERSION 3.7)

find_library(LibGCrypt 
    NAMES libgcrypt.so libgcrypt.a
    PATHS "/usr/lib/x86_64-linux-gnu")

find_library(LibConfig 
    NAMES libconfig++.so libconfig++.a
    PATHS "/usr/lib/x86_64-linux-gnu")

include_directories("src"
     /usr/local/include
     /usr/local/include/cppconn
     ${CMAKE_BINARY_DIR}
     /usr/include
     /usr/include/mysql
     /usr/include/mysqlx
     /usr/include/jdbc
     )

set(ANUS_SOURCES "src/client_config_handler.cpp"
                 "src/server_config_handler.cpp"
                 )
add_library(anus ${ANUS_SOURCES})
target_link_libraries(anus mysqlcppconn middleware)

add_executable(test_config_generation main.cpp)

target_link_libraries(test_config_generation anus)
target_link_libraries(test_config_generation ${LibGCrypt})
target_link_libraries(test_config_generation ${LibConfig})
