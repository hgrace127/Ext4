cmake_minimum_required(VERSION 3.16)
project(Ext4)

set(CMAKE_CXX_STANDARD 17)

include_directories(develop/include)

SET (TEST_SOURCES
  Test/main.cpp
  src/Ext4.cpp
  src/INode.cpp
  src/Node.cpp
  src/NodeList.cpp
  src/Extent.cpp
  src/Ext4_Extent.cpp
  src/Superblock.cpp
  src/BlockGroupDescriptor.cpp
  src/DirectoryEntry.cpp
  develop/src/byte_buffer2.cpp
)

ADD_DEFINITIONS(-DPOSIX)

INCLUDE_DIRECTORIES (
  include
  develop/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE (Test ${TEST_SOURCES})
