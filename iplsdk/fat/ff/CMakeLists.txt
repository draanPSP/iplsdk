add_library(ff_rw STATIC)
add_library(iplsdk::ff_rw ALIAS ff_rw)

target_include_directories(ff_rw PUBLIC include)

# RW mode
target_compile_definitions(ff_rw PUBLIC FF_FS_READONLY=0)
# Leave f_opendir(), f_readdir() and f_closedir() in
target_compile_definitions(ff_rw PUBLIC FF_FS_MINIMIZE=1)
# Handle Long File Names on stack
target_compile_definitions(ff_rw PUBLIC FF_USE_LFN=2)
# Use UTF8 in Long File Names
target_compile_definitions(ff_rw PUBLIC FF_LFN_UNICODE=2)
# Use tiny file buffers
target_compile_definitions(ff_rw PUBLIC FF_FS_TINY=1)

target_sources(ff_rw PRIVATE ff.c diskio.c ffunicode.c)

target_link_libraries(ff_rw PRIVATE iplsdk::ms)


add_library(ff_ro STATIC)
add_library(iplsdk::ff_ro ALIAS ff_ro)

target_include_directories(ff_ro PUBLIC include)

# RO mode
target_compile_definitions(ff_ro PUBLIC FF_FS_READONLY=1)
# Maximum minimization
target_compile_definitions(ff_ro PUBLIC FF_FS_MINIMIZE=3)
# Handle Long File Names on stack
target_compile_definitions(ff_ro PUBLIC FF_USE_LFN=2)
# Use UTF8 in Long File Names
target_compile_definitions(ff_rw PUBLIC FF_LFN_UNICODE=2)
# Use tiny file buffers
target_compile_definitions(ff_ro PUBLIC FF_FS_TINY=1)

target_sources(ff_ro PRIVATE ff.c diskio.c ffunicode.c)

target_link_libraries(ff_ro PRIVATE iplsdk::ms)
