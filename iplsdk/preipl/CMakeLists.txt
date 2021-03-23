add_library(preipl INTERFACE)
add_library(iplsdk::preipl ALIAS preipl)

target_include_directories(preipl INTERFACE include)

target_sources(preipl INTERFACE include/preipl.h)