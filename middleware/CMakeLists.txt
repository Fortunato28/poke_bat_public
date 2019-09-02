cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

set(THRIFT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/interfaces.thrift")

find_program(CMAKE_CXX_THRIFT NAMES thrift)

if(CMAKE_CXX_THRIFT)
     set(THRIFT_COMMAND ${CMAKE_CXX_THRIFT} -r --gen cpp ${THRIFT_FILE})
     execute_process(COMMAND ${THRIFT_COMMAND} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
     message(STATUS "Thrift files was sucsessfully generated")
else(CMAKE_CXX_THRIFT)
     message(FATAL_ERROR "Cannot find thrift in path")
endif()

include_directories(/usr/local/include/ ${CMAKE_BINARY_DIR}/ .)

set(THRIFT_GENERATED
    ${CMAKE_BINARY_DIR}/gen-cpp/interfaces_constants.cpp
    ${CMAKE_BINARY_DIR}/gen-cpp/interfaces_types.cpp
    ${CMAKE_BINARY_DIR}/gen-cpp/PokServer.cpp
    )
set(MIDDLEWARE_SOURCES
    server_controller.cpp
    pok_server.cpp
    )

add_library(middleware ${MIDDLEWARE_SOURCES} ${THRIFT_GENERATED})
target_link_libraries(middleware thrift thriftnb)

add_executable(test_thrift main.cpp)
target_link_libraries(test_thrift middleware)
