include_directories(../middleware
    /usr/local/include 
    /usr/local/include/cppconn
    ${CMAKE_BINARY_DIR}
    /usr/include
    /usr/include/mysql
    /usr/include/mysqlx
    /usr/include/jdbc
    ${CMAKE_SOURCE_DIR}/include
    )

set(DB_MANAGER_SOURCES database_manager.cpp)

add_library(db_manager ${DB_MANAGER_SOURCES})
target_link_libraries(db_manager mysqlcppconn middleware)

add_executable(db_test main.cpp)
target_link_libraries(db_test db_manager)
