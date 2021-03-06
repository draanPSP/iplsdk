cmake_minimum_required(VERSION 3.17)

project(timemachine VERSION 1.0 LANGUAGES C CXX ASM)

set (CMAKE_CXX_STANDARD 20)

add_compile_options(-Os -fno-exceptions $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)

add_subdirectory(iplsdk)
add_subdirectory(flash_led)
