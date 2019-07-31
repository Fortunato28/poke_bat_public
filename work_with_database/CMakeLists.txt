cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories(/usr/local/include /usr/local/include/cppconn)

set(DB_MANAGER_SOURCES database_manager.cpp)

add_library(db_manager ${DB_MANAGER_SOURCES})
target_link_libraries(db_manager mysqlcppconn)

add_executable(test_database main.cpp)
target_link_libraries(test_database db_manager)
