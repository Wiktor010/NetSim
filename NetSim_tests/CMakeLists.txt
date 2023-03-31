cmake_minimum_required(VERSION 3.17)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

set(This NetSim__tests)

set(Sources
        main_gtest.cpp
        test_package.cpp
        )

add_library(import_setup
        ../src/factory.cpp
        ../src/helpers.cpp
        ../src/nodes.cpp
        ../src/package.cpp
        ../src/reports.cpp
        ../src/simulation.cpp
        ../src/types.cpp)

add_executable(${This} ${Sources})


target_link_libraries(${This} PUBLIC
        gtest_main
        import_setup
        )

add_test(
        NAME ${This}
        COMMAND ${This}
)