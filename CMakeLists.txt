cmake_minimum_required(VERSION 3.10)

project(TCPServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(server
    src/server.cpp
    src/logger/logger.cpp
    src/router/router.cpp
    src/config/config.cpp
    src/threadpool/thread_pool.cpp
    src/http/response.cpp
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(unit_tests
    tests/router_test.cpp
    tests/response_test.cpp
    tests/config_test.cpp

    src/router/router.cpp
    src/http/response.cpp
    src/config/config.cpp
)

target_link_libraries(unit_tests
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME unit_tests COMMAND unit_tests)