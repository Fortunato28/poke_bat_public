cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories(gen-cpp/)

set(THRIFT_GENERATED 
    gen-cpp/interfaces_constants.cpp
    gen-cpp/interfaces_types.cpp
    gen-cpp/PokServer.cpp
    )
set(MIDDLEWARE_SOURCES 
    test.cpp
    gen-cpp/PokServer.cpp
    )

add_library(middleware ${MIDDLEWARE_SOURCES} ${THRIFT_GENERATED})
target_link_libraries(middleware mysqlcppconn)

add_executable(test_thrift main.cpp)
target_link_libraries(test_thrift middleware)
