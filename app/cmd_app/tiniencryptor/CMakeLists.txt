cmake_minimum_required(VERSION 3.16)
project(tiniencryptor)

set(CMAKE_CXX_STANDARD 17)

include_directories(cmd_app/headers)
include_directories(Dependencies/argparse/include)

add_executable(tiniencryptor
        cmd_app/src/cmd_app.cpp
        cmd_app/src/huffman.cpp
        cmd_app/src/util.cpp)
