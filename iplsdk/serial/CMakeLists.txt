add_library(serial STATIC)
add_library(iplsdk::serial ALIAS serial)

target_include_directories(serial PUBLIC include)

target_sources(serial PRIVATE serial.cpp printf.c)

#disable extra format specifiers for less code
target_compile_definitions(serial PRIVATE PRINTF_DISABLE_SUPPORT_FLOAT PRINTF_DISABLE_SUPPORT_EXPONENTIAL PRINTF_DISABLE_SUPPORT_LONG_LONG)

target_link_libraries(serial PRIVATE iplsdk::hw iplsdk::lowio)
