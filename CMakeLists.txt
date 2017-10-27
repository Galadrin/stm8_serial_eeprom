
cmake_minimum_required(VERSION 3.5)

project(lt200_dataplug C)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stm8_hal)

add_definitions(-DSTM8S003)

ADD_EXECUTABLE(main src/lt200_dongle.c src/eeprom.c src/crc16.c stm8_hal/stm8s_gpio.c stm8_hal/stm8s_clk.c stm8_hal/stm8s_tim4.c stm8_hal/stm8s_uart1.c)

add_custom_command(TARGET main
        POST_BUILD
        COMMAND  ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/src/ "C:/TargetDirectory")

# Enable/Disable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)