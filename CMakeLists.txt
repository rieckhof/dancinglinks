cmake_minimum_required(VERSION 2.6)
project(sudoku_solver)
option(COVERAGE "Enable coverage" OFF)
find_package(Threads REQUIRED)

if(COVERAGE)
    message("Coverage activated")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -Wall -fprofile-arcs -ftest-coverage" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    add_custom_target(
                coverage
                COMMAND ${LCOV} -b ${PROJECT_SOURCE_DIR}
                                -c -d ${CMAKE_BINARY_DIR} -o coverage.run
                COMMAND ${LCOV} -e coverage.run \"${PROJECT_SOURCE_DIR}/*\"
                                -o coverage.run.filtered
                COMMAND ${LCOV} -r coverage.run.filtered \"*/googletest-src/*\" -o coverage.run.filtered
                COMMAND ${GENHTML} --branch-coverage -o ./html/ coverage.run.filtered
            )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=c++17" )
endif()
unset(COVERAGE)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
