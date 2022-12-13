add_library(kirk STATIC)
add_library(iplsdk::kirk ALIAS kirk)

target_include_directories(kirk PUBLIC include)

target_sources(kirk PRIVATE kirk.cpp)

target_link_libraries(kirk PRIVATE iplsdk::hw iplsdk::lowio iplsdk::cache)
