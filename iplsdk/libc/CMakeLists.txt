add_library(libc INTERFACE)
add_library(iplsdk::libc ALIAS libc)

target_include_directories(libc INTERFACE include)

target_sources(libc INTERFACE include/libc.h)