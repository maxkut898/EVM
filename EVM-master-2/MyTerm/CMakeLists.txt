cmake_minimum_required(VERSION 3.6)
project(MyTerm)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/MyTerm.c include/MyTerm.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(MyTerm STATIC ${SOURCE_FILES})