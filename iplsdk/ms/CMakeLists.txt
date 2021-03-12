add_library(ms STATIC)
add_library(iplsdk::ms ALIAS ms)

target_include_directories(ms PUBLIC include)

target_sources(ms PRIVATE ms.cpp)

target_link_libraries(ms iplsdk::hw iplsdk::lowio)
