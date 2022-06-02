cmake_minimum_required(VERSION 3.6)
project(MySimpleComputer)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/MySimpleComputer.c include/MySimpleComputer.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(MySimpleComputer STATIC ${SOURCE_FILES})
target_link_libraries(MySimpleComputer m)