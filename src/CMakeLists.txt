add_library(myLib serializer.h
    serializer.cpp dancinglinks.h dancinglinks.cpp
    dancinglinks_solver.h dancinglinks_solver.cpp )

add_executable(s_solver main.cpp)
target_link_libraries(s_solver myLib Threads::Threads)
