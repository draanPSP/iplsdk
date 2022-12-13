add_executable(nand_info)

target_sources(nand_info PRIVATE main.cpp)

target_link_libraries(nand_info PRIVATE iplsdk::crt iplsdk::lowio iplsdk::hw iplsdk::syscon iplsdk::serial iplsdk::nand)

add_custom_command(
    TARGET nand_info
    POST_BUILD
    COMMAND psp-objcopy -O binary nand_info nand_info.bin
    COMMAND python3 ${PROJECT_SOURCE_DIR}/convert_to_tm.py --infile nand_info.bin --outfile nand_info_tm.bin
)

target_link_options(nand_info PRIVATE -nostdlib -T${PROJECT_BINARY_DIR}/linker.x)
