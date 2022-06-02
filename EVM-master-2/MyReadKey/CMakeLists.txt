cmake_minimum_required(VERSION 3.6)
project(MyReadKey)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/MyReadKey.c include/MyReadKey.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(MyReadKey STATIC ${SOURCE_FILES})