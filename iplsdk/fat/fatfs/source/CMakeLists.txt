add_library(fatfs STATIC)
add_library(iplsdk::fatfs ALIAS fatfs)

target_include_directories(fatfs PUBLIC .)

target_sources(fatfs PRIVATE ffsystem.c ff.c diskio_psp.c ffunicode.c)

target_link_libraries(fatfs c iplsdk::ms)
