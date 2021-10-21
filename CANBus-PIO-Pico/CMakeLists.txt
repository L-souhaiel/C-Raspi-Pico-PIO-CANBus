cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project (CAN_Rx C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(CAN_Rx)

pico_enable_stdio_usb(CAN_Rx 1)
pico_enable_stdio_uart(CAN_Rx 0) 


pico_generate_pio_header(CAN_Rx ${CMAKE_CURRENT_LIST_DIR}/CAN_Rx.pio)

target_sources(CAN_Rx PRIVATE  CAN_Rx.c CAN_RX_D.h CAN.c)

target_link_libraries(CAN_Rx PRIVATE pico_stdlib  hardware_pio pico_multicore  pico_sync )

pico_add_extra_outputs(CAN_Rx)