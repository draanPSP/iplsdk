add_library(crt OBJECT)
add_library(iplsdk::crt ALIAS crt)

target_sources(crt PRIVATE crt0.s)

add_library(linker_script INTERFACE)
add_library(iplsdk::linker_script ALIAS linker_script)

target_sources(linker_script INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/linker.x)
