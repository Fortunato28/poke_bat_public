cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories(/usr/local/include /usr/local/include/cppconn)

set(DB_MANAGER_SOURCES database_manager.cpp)

find_library(MIDDLEWARE_LIB middleware ~/Programming/cpp/poke_bat/middleware/_result)
add_library(db_manager ${DB_MANAGER_SOURCES})
target_link_libraries(db_manager mysqlcppconn middleware)

add_executable(db_test main.cpp)
target_link_libraries(db_test db_manager)
