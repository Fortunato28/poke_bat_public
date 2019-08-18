cmake_minimum_required(VERSION 3.7) # Проверка версии CMake.

# TODO Add complitaion flags and so on

add_subdirectory(work_with_database)
add_subdirectory(config_generation)
add_subdirectory(middleware)

add_executable(main main.cpp)
