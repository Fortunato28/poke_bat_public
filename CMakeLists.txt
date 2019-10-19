cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

set (POKEBAT_INCLUDE_DIRS "")
list (APPEND POKEBAT_INCLUDE_DIRS "mysql-connector")
list (APPEND POKEBAT_INCLUDE_DIRS "mysql-connector/driver")
list (APPEND POKEBAT_INCLUDE_DIRS "mysql-connector/cppconn")
list (APPEND POKEBAT_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/mysql-connector/cppconn)
list (APPEND POKEBAT_INCLUDE_DIRS "middleware")
list (APPEND POKEBAT_INCLUDE_DIRS "pokebat_config_parser")

# TODO Add complitaion flags and so on
add_subdirectory(mysql-connector)
add_subdirectory(work_with_database)
add_subdirectory(config_generation)
add_subdirectory(pokebat_config_parser)
add_subdirectory(middleware)

include_directories(
                    ${CMAKE_BINARY_DIR}
                    ${POKEBAT_INCLUDE_DIRS}
                    work_with_database
                    pokebat_config_parser
                    config_generation
                    include/
                    /usr/include
                    /usr/include/mysql
                    /usr/include/mysqlx
                    /usr/include/jdbc
                    .
                    )

# Сервер
set(SERVER_EXAMPLE_SOURCES
    src/pok_server.cpp
    src/server_controller.cpp
    src/server.cpp
    src/fight.cpp)

add_executable(server ${SERVER_EXAMPLE_SOURCES})
target_link_libraries(server
                      middleware
                      config_generation
                      pokebat_config_parser
                      db_manager
                      )

# Клиент
set(CLIENT_EXAMPLE_SOURCES
    src/client.cpp)

add_executable(client ${CLIENT_EXAMPLE_SOURCES})
target_link_libraries(client middleware config_generation)
