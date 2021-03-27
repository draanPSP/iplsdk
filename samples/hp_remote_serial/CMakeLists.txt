add_executable(hp_remote_serial)

target_sources(hp_remote_serial PRIVATE main.cpp)

target_link_libraries(hp_remote_serial PRIVATE iplsdk::crt iplsdk::lowio iplsdk::serial iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(hp_remote_serial PRIVATE -flto)

add_custom_command(
    TARGET hp_remote_serial
    POST_BUILD
    COMMAND psp-objcopy -O binary hp_remote_serial hp_remote_serial.bin
    COMMAND python3 ${PROJECT_SOURCE_DIR}/convert_to_tm.py --infile hp_remote_serial.bin --outfile hp_remote_serial_tm.bin
)

target_link_options(hp_remote_serial PRIVATE -nostdlib -T${PROJECT_BINARY_DIR}/linker.x)
