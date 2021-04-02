cmake_minimum_required(VERSION 3.16)

project(iplsdk VERSION 1.0 LANGUAGES C CXX ASM)

set (CMAKE_CXX_STANDARD 20)

add_compile_options(-Os -fno-exceptions $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)

add_subdirectory(iplsdk)

#if iplsdk is the main project, also build samples
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(samples)

endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
