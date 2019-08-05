cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories()

set(THRIFT_SOURCES test.cpp)

add_library(middleware ${THRIFT_SOURCES})
target_link_libraries(middleware mysqlcppconn)

add_executable(test_thrift main.cpp)
target_link_libraries(test_thrift middleware)
