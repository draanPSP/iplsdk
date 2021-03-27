add_library(lowio STATIC)
add_library(iplsdk::lowio ALIAS lowio)

target_include_directories(lowio PUBLIC include)

target_sources(lowio PRIVATE lowio.cpp)

target_link_libraries(lowio PUBLIC iplsdk::hw)
