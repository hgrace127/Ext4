cmake_minimum_required(VERSION 3.16)
project(Ext4)

set(CMAKE_CXX_STANDARD 14)

include_directories(develop/include)

add_executable(Ext4
        develop/include/byte_buffer2.hpp
        develop/include/endian_swap.hpp
        develop/src/byte_buffer2.cpp
        src/Ext4.cpp
        src/iNode.cpp
        src/Node.cpp
        src/Superblock.cpp
        Test/main.cpp)
