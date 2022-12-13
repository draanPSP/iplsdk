add_library(nand STATIC)
add_library(iplsdk::nand ALIAS nand)

target_include_directories(nand PUBLIC include)

target_sources(nand PRIVATE nand.cpp)

target_link_libraries(nand PRIVATE iplsdk::hw iplsdk::lowio)
