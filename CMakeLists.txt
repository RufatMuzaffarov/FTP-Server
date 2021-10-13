cmake_minimum_required (VERSION 3.5)
project (FTP-Server)

SET (CMAKE_CXX_FLAGS_DEBUG " -O0 -Wall -Werror -g -ggdb -D_ONLY_TEST")
SET (CMAKE_C_FLAGS_DEBUG   " -O0 -Wall -Werror -g -ggdb -D_ONLY_TEST")
SET (CMAKE_BUILD_TYPE Debug)

SET (CMAKE_CXX_COMPILER   "ccache")
SET (CMAKE_CXX_COMPILER_ARG1 "g++")
SET (CMAKE_C_COMPILER     "ccache")
SET (CMAKE_C_COMPILER_ARG1   "gcc")

INCLUDE_DIRECTORIES (
    .
)

SET (SRC_LIST
    EvLoopTask.cpp
    FTPTask.cpp
    MemBlock.cpp
    SendTask.cpp
    SingleFactory.cpp
    ThreadPool.cpp
    Main.cpp
)

SET (EXTRA_LIBS
   -lpthread
   -lrt
   -lev
)

add_executable (FTP-Server ${SRC_LIST})
target_link_libraries (FTP-Servers ${EXTRA_LIBS})
