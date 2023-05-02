cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD  20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

project(testapp)

add_library(network_utils network_utils.cpp)

add_executable(testapp testapp.cpp TcpClientDbManager.cpp TcpClientServiceManager.cpp TcpNewConnectionAccepter.cpp TcpServerController.cpp TcpClient.cpp)
target_link_libraries(testapp network_utils)