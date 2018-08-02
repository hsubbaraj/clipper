# Define library. Only source files here!
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(selectionpolicy VERSION 0.1.0 LANGUAGES CXX C)

add_library(selectionpolicy
        src/rpc_handle.cpp
        src/rpc_handle_wrapper.cpp)

target_include_directories(selectionpolicy
        PRIVATE src)

target_link_libraries(selectionpolicy clipper zmqcpp cqueue cxxopts)