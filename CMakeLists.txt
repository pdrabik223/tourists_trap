cmake_minimum_required(VERSION 3.19)
project(turists_trap)

set(CMAKE_CXX_STANDARD 14)

add_executable(turists_trap
        kruskala/main.cpp)

add_executable(for_lizzy kruskala/liz_main.cpp
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp

        )

add_executable(lizzy kruskala/liz_main_v02.cpp
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp
       )

add_executable(prim prima/ASD2gr2_Drabik_Piotr_MST.cpp)
add_executable(liz_prim liz_prim/main.cpp)
add_executable(gen prima/gen_data.cpp)