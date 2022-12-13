add_executable(kirk_decrypt_ipl)

target_sources(kirk_decrypt_ipl PRIVATE main.cpp)

target_link_libraries(kirk_decrypt_ipl PRIVATE iplsdk::crt iplsdk::lowio iplsdk::hw iplsdk::syscon iplsdk::serial iplsdk::kirk iplsdk::nand)

add_custom_command(
    TARGET kirk_decrypt_ipl
    POST_BUILD
    COMMAND psp-objcopy -O binary kirk_decrypt_ipl kirk_decrypt_ipl.bin
    COMMAND python3 ${PROJECT_SOURCE_DIR}/convert_to_tm.py --infile kirk_decrypt_ipl.bin --outfile kirk_decrypt_ipl_tm.bin
)

target_link_options(kirk_decrypt_ipl PRIVATE -nostdlib -T${PROJECT_BINARY_DIR}/linker.x)
