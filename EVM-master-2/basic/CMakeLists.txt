cmake_minimum_required(VERSION 3.6)
project(basic)

set(CMAKE_C_STANDARD 11)
set(CF_HEADERS include/basic.h include/rpn.h ../asm/include/sat.h)
set(CF_SOURCES src/basic.c src/rpn.c ../asm/src/sat.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${MySimpleComputer_SOURCE_DIR}/include)
include_directories(${asm_SOURCE_DIR}/include)

add_executable(basic ${CF_HEADERS} ${CF_SOURCES})
target_link_libraries(${PROJECT_NAME} MySimpleComputer)