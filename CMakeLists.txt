
cmake_minimum_required(VERSION 3.5)

project(lt200_dataplug C)

set(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(main src/main.c)

include_directories(/opt/cross-tools/share/sdcc/include/)


# Enable/Disable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
