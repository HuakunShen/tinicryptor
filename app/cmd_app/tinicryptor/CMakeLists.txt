cmake_minimum_required(VERSION 3.16)
project(tinicryptor)

set(CMAKE_CXX_STANDARD 17)

include_directories(cmd_app/headers)
include_directories(Dependencies/argparse/include)

add_executable(tinicryptor
        cmd_app/headers/decode.hpp
        cmd_app/headers/encode.hpp
        cmd_app/headers/huffman.hpp
        cmd_app/headers/util.hpp
        cmd_app/src/cmd_app.cpp
        cmd_app/src/decode.cpp
        cmd_app/src/encode.cpp
        cmd_app/src/huffman.cpp
        cmd_app/src/util.cpp
        Dependencies/argparse/include/argparse/argparse.hpp)
