
cmake_minimum_required(VERSION 3.5)

project(lt200_dataplug C)

set(CMAKE_CXX_STANDARD 11)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
#include_directories(/opt/cross-tools/share/sdcc/include/)

ADD_EXECUTABLE(main src/main.c inc/stm8s.h)

# Enable/Disable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)