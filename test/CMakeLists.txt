include(../DownloadProject-master/DownloadProject.cmake)

download_project(
    PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

add_executable(SerializerTest serializer_test.cpp)
add_executable(DancingLinksTest dancinglinks_test.cpp utils.h)
add_executable(DancingLinksSolverTest dancinglinks_solver_test.cpp utils.h)
add_executable(DancingLinksIntegrationTest dancinglinks_integration_test.cpp utils.h)

target_link_libraries(SerializerTest gtest gmock_main myLib)
target_link_libraries(DancingLinksTest gtest gmock_main myLib)
target_link_libraries(DancingLinksSolverTest gtest gmock_main myLib)
target_link_libraries(DancingLinksIntegrationTest gtest gmock_main myLib)

configure_file(${PROJECT_SOURCE_DIR}/test/sudoku ${PROJECT_BINARY_DIR}/test/sudoku)
configure_file(${PROJECT_SOURCE_DIR}/test/sudoku99 ${PROJECT_BINARY_DIR}/test/sudoku99)

add_test(SerializerTest SerializerTest)
add_test(DancingLinksTest DancingLinksTest)
add_test(DancingLinksSolverTest DancingLinksTest)
add_test(DancingLinksIntegrationTest DancingLinksIntegrationTest)
