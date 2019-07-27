cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

list(APPEND HEADERS "src")

list(APPEND SOURCES "src/configHandler.h")
list(APPEND SOURCES "src/configHandler.cpp")
list(APPEND SOURCES "main.cpp")

include_directories(${HEADERS})

add_executable(test_config_generation ${SOURCES})
