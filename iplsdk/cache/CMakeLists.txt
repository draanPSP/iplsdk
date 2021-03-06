add_library(cache STATIC)
add_library(iplsdk::cache ALIAS cache)

target_include_directories(cache PUBLIC include)

target_sources(cache PRIVATE cache.cpp)