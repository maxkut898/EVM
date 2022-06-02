cmake_minimum_required(VERSION 3.6)
project(asm)

set(CMAKE_C_STANDARD 11)
set(CF_HEADERS include/sat.h)
set(CF_SOURCES src/sat.c src/main.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${MySimpleComputer_SOURCE_DIR}/include)

add_executable(asm ${CF_HEADERS} ${CF_SOURCES})
target_link_libraries(${PROJECT_NAME} MySimpleComputer)