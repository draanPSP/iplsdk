add_library(syscon STATIC)
add_library(iplsdk::syscon ALIAS syscon)

target_include_directories(syscon PUBLIC include)

target_sources(syscon PRIVATE syscon.cpp)

target_link_libraries(syscon c iplsdk::hw iplsdk::lowio)