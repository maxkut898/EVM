cmake_minimum_required(VERSION 3.6)
project(MyBigChars)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/MyBigChars.c include/MyBigChars.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${MyTerm_SOURCE_DIR}/include)

add_library(MyBigChars STATIC ${SOURCE_FILES})
target_link_libraries(MyBigChars MyTerm)