add_library(pff STATIC)
add_library(iplsdk::pff ALIAS pff)

target_include_directories(pff PUBLIC include)

target_sources(pff PRIVATE pff.c diskio.c)

target_link_libraries(pff c iplsdk::ms)
