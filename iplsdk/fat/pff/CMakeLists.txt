add_library(pff_rw STATIC)
add_library(iplsdk::pff_rw ALIAS pff_rw)

target_include_directories(pff_rw PUBLIC include)

target_sources(pff_rw PRIVATE pff.c diskio.c)

target_link_libraries(pff_rw PRIVATE c iplsdk::ms)

target_compile_definitions(pff_rw PUBLIC PF_USE_READ=1)
target_compile_definitions(pff_rw PUBLIC PF_USE_DIR=1)
target_compile_definitions(pff_rw PUBLIC PF_USE_LSEEK=0)
target_compile_definitions(pff_rw PUBLIC PF_USE_WRITE=1)

add_library(pff_ro STATIC)
add_library(iplsdk::pff_ro ALIAS pff_ro)

target_include_directories(pff_ro PUBLIC include)

target_sources(pff_ro PRIVATE pff.c diskio.c)

target_link_libraries(pff_ro PRIVATE c iplsdk::ms)

target_compile_definitions(pff_ro PUBLIC PF_USE_READ=1)
target_compile_definitions(pff_ro PUBLIC PF_USE_DIR=0)
target_compile_definitions(pff_ro PUBLIC PF_USE_LSEEK=0)
target_compile_definitions(pff_ro PUBLIC PF_USE_WRITE=0)
