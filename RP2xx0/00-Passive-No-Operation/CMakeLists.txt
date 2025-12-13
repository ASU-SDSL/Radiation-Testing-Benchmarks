cmake_minimum_required(VERSION 3.12)

# get SDK
include(pico_sdk_import.cmake)
# include(pico_extras_import_optional.cmake)

project(passive_no_operation C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.1.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.1.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

#init sdk
pico_sdk_init() 

# include(example_auto_set_url.cmake)

add_executable(${PROJECT_NAME}
  main.cpp
)

# doesn't need libraries
target_link_libraries(${PROJECT_NAME}
  pico_stdlib
)

# passive power should be on flash - it doesn't really matter if it gets corrupted 
# but something should be there to run 
# pico_set_binary_type(${PROJECT_NAME} no_flash) 

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

# example_auto_set_url(radio_test)