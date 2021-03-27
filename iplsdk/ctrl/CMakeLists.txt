add_library(ctrl STATIC)
add_library(iplsdk::ctrl ALIAS ctrl)

target_include_directories(ctrl PUBLIC include)

target_sources(ctrl PRIVATE ctrl.cpp)

target_link_libraries(ctrl PRIVATE iplsdk::syscon)