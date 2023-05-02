cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD  20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

project(tcp_client)

add_library(network_utils ../network_utils.cpp)
add_executable(tcp_client tcp_client.cpp)
#add_executable(test_2 tcp_client_2.cpp)
target_link_libraries(tcp_client network_utils)
#target_link_libraries(test_2 network_utils)

