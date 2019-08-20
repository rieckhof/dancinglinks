add_library(myLib sudoku_matrix.cpp sudoku_matrix.h board.cpp board.h serializer.h
    serializer.cpp solver.h solver.cpp utils.cpp utils.h dancinglinks.h dancinglinks.cpp)
target_include_directories(myLib PRIVATE ${PROJECT_SOURCE_DIR}/jsonparser/include)

add_executable(s_solver main.cpp)
target_link_libraries(s_solver myLib Threads::Threads)
