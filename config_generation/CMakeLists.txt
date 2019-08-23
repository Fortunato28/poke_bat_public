cmake_minimum_required(VERSION 3.7)


#KILL_IT
set(THRIFT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/interfaces.thrift")

find_program(CMAKE_CXX_THRIFT NAMES thrift)

if(CMAKE_CXX_THRIFT)
     set(THRIFT_COMMAND ${CMAKE_CXX_THRIFT} -r --gen cpp ${THRIFT_FILE})
     execute_process(COMMAND ${THRIFT_COMMAND} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
     message(STATUS "Thrift files was sucsessfully generated")
else(CMAKE_CXX_THRIFT)
     message(FATAL_ERROR "Cannot find thrift in path")
endif()

include_directories(${CMAKE_BINARY_DIR}/gen-cpp/)

set(THRIFT_GENERATED
    ${CMAKE_BINARY_DIR}/gen-cpp/interfaces_constants.cpp
    ${CMAKE_BINARY_DIR}/gen-cpp/interfaces_types.cpp
    ${CMAKE_BINARY_DIR}/gen-cpp/PokServer.cpp
    )
set(MIDDLEWARE_SOURCES
    test.cpp
    )

add_library(middleware ${MIDDLEWARE_SOURCES} ${THRIFT_GENERATED})
#KILL_END


find_library(LibGCrypt 
    NAMES libgcrypt.so libgcrypt.a
    PATHS "/usr/lib/x86_64-linux-gnu")

find_library(LibConfig 
    NAMES libconfig++.so libconfig++.a
    PATHS "/usr/lib/x86_64-linux-gnu")

#list of dirs with headers
list(APPEND HEADERS_DIRS "src")

#list of headers
list(APPEND HEADERS "src/client_config_handler.h")
list(APPEND HEADERS "src/server_config_handler.h")

#list of sources
list(APPEND SOURCES "src/client_config_handler.cpp")
list(APPEND SOURCES "src/server_config_handler.cpp")
list(APPEND SOURCES "main.cpp")

include_directories(${HEADERS_DIRS})

add_executable(test_config_generation ${HEADERS} ${SOURCES})

target_link_libraries(test_config_generation ${LibGCrypt})
target_link_libraries(test_config_generation ${LibConfig})
target_link_libraries(test_thrift middleware)
