project(cloc)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

SET(CMAKE_CXX_FLAGS "-std=c++1y -Wall -g -lboost_filesystem -lboost_system -lboost_regex")
SET(CMAKE_CXX_COMPILER, "clang++")
