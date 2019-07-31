cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories(/usr/include/mysql++ /usr/include/mysql)

add_executable(test_database main.cpp database_manager.cpp)
