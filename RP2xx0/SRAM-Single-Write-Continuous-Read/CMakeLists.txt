cmake_minimum_required(VERSION 3.12)

# get SDK
include(pico_sdk_import.cmake)
# include(pico_extras_import_optional.cmake)

project(sram_single_write_continuous_read C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#init sdk
pico_sdk_init() 

# include(example_auto_set_url.cmake)

add_executable(${PROJECT_NAME}
  main.cpp
)

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  hardware_spi
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

# example_auto_set_url(radio_test)