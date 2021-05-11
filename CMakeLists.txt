cmake_minimum_required(VERSION 3.19)
project(turists_trap)

set(CMAKE_CXX_STANDARD 14)

add_executable(turists_trap
        main.cpp)

add_executable(for_lizzy
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp )

add_executable(lizzy lizzy/liz_main_v02.cpp
        windows_console_tools/win_colors.h windows_console_tools/win_colors.cpp
        lizzy/node.h lizzy/node.cpp
        lizzy/tree.h lizzy/tree.cpp)