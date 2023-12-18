cmake_minimum_required(VERSION 3.17)
project(CStreamServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(CStreamServer main.cpp CStreamServerHeaderDef.h CStreamServer.cpp CStreamServer.h CStreamProcess.cpp CStreamProcess.h CThread.h CThread.cpp)