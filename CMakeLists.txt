cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

# TODO Add complitaion flags and so on

add_subdirectory(work_with_database)
add_subdirectory(config_generation)
add_subdirectory(middleware)

include_directories(middleware ${CMAKE_BINARY_DIR} include/)

# Сервер
set(SERVER_EXAMPLE_SOURCES
    src/pok_server.cpp
    src/server_controller.cpp
    src/server.cpp)

add_executable(server ${SERVER_EXAMPLE_SOURCES})
target_link_libraries(server middleware)

# Клиент
set(CLIENT_EXAMPLE_SOURCES
    src/client.cpp)

add_executable(client ${CLIENT_EXAMPLE_SOURCES})
target_link_libraries(client middleware)
