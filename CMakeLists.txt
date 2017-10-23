
cmake_minimum_required(VERSION 3.5)

project(lt200_dataplug C)

set(CMAKE_CXX_STANDARD 11)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
#include_directories(/opt/cross-tools/share/sdcc/include/)

ADD_EXECUTABLE(main src/lt200_dongle.c src/clock.c src/gpio.c src/eeprom.c inc/eeprom.h src/crc16.c inc/crc16.h inc/typedef.h)

add_custom_command(TARGET main
        POST_BUILD
        COMMAND  ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/src/ "C:/TargetDirectory")

# Enable/Disable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)