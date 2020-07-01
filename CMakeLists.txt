CMAKE_MINIMUM_REQUIRED (VERSION 3.15)

SET (CMAKE_CXX_STANDARD 17)

PROJECT (Ext4)

SET (TEST_SOURCES
  Test/main.cpp
  src/Ext4.cpp
  src/iNode.cpp
  src/Node.cpp
  src/Superblock.cpp
  develop/src/byte_buffer2.cpp
)

ADD_DEFINITIONS(-DPOSIX)

INCLUDE_DIRECTORIES (
  develop/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE (Test ${TEST_SOURCES})