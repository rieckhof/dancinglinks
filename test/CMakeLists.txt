include(../DownloadProject-master/DownloadProject.cmake)

download_project(
    PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/jsonparser/include)

add_executable(MatrixTest data_test.cpp)
add_executable(BoardTest board_test.cpp)
add_executable(SerializerTest serializer_test.cpp)
add_executable(SolverTest solver_test.cpp)
add_executable(DancingLinksTest dancinglinks_test.cpp utils.h)
add_executable(DancingLinksSolverTest dancinglinks_solver_test.cpp utils.h)

target_link_libraries(MatrixTest gtest gmock_main myLib)
target_link_libraries(BoardTest gtest gmock_main myLib)
target_link_libraries(SerializerTest gtest gmock_main myLib)
target_link_libraries(SolverTest gtest gmock_main myLib)
target_link_libraries(DancingLinksTest gtest gmock_main myLib)
target_link_libraries(DancingLinksSolverTest gtest gmock_main myLib)

configure_file(${PROJECT_SOURCE_DIR}/test/sudoku ${PROJECT_BINARY_DIR}/test/sudoku)
add_test(MatrixTest MatrixTest)
add_test(BoardTest BoardTest)
add_test(SerializerTest SerializerTest)
add_test(SolverTest SolverTest)
add_test(DancingLinksTest DancingLinksTest)
add_test(DancingLinksSolverTest DancingLinksTest)
