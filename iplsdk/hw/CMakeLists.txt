add_library(hw INTERFACE)
add_library(iplsdk::hw ALIAS hw)

target_include_directories(hw INTERFACE include)

target_sources(hw INTERFACE include/hw.h)
