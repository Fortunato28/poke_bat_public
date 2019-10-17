include_directories(../middleware
    /usr/local/include
    /usr/local/include/cppconn
    ${CMAKE_BINARY_DIR}
    /usr/include
    /usr/include/mysql
    /usr/include/mysqlx
    /usr/include/jdbc
    ${CMAKE_SOURCE_DIR}/include
    .
    ../mysql-connector
    ../mysql-connector/cppconn
    ../mysql-connector/driver
    ${CMAKE_CURRENT_BINARY_DIR}/../mysql-connector/cppconn
    )

set(DB_MANAGER_SOURCES
    database_manager.cpp )

add_library(db_manager ${DB_MANAGER_SOURCES})
target_link_libraries(db_manager mysqlcppconn-static middleware)

add_executable(db_test main.cpp)
target_link_libraries(db_test db_manager)
