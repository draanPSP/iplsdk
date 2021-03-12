add_library(tff STATIC)
add_library(iplsdk::tff ALIAS tff)

target_include_directories(tff PUBLIC include)

target_sources(tff PRIVATE diskio.c tff.c)

target_link_libraries(tff iplsdk::ms iplsdk::serial)
