cmake_minimum_required(VERSION 3.19)
project(turists_trap)

set(CMAKE_CXX_STANDARD 14)

add_executable(turists_trap
        main.cpp)

add_executable(for_lizzy  liz_main.cpp
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp
        my_vector.h my_vector.cpp
        )

add_executable(lizzy  liz_main_v02.cpp
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp )