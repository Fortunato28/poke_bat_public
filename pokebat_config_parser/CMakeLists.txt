cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

set(POKEBAT_CONFIG_PARSER_SOURCES
    config_parser.cpp )

add_library(pokebat_config_parser ${POKEBAT_CONFIG_PARSER_SOURCES})
target_link_libraries(pokebat_config_parser)

add_executable(pokebat_config_parser_test main.cpp)
target_link_libraries(pokebat_config_parser_test pokebat_config_parser)
