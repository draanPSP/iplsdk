add_executable(memory_stick_ro)

target_sources(memory_stick_ro PRIVATE main.cpp)

target_link_libraries(memory_stick_ro PRIVATE iplsdk::crt iplsdk::pff_ro iplsdk::serial iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(memory_stick_ro PRIVATE -flto)

add_custom_command(
    TARGET memory_stick_ro
    POST_BUILD
    COMMAND psp-objcopy -O binary memory_stick_ro memory_stick_ro.bin
    COMMAND python3 ${PROJECT_SOURCE_DIR}/convert_to_tm.py --infile memory_stick_ro.bin --outfile memory_stick_ro_tm.bin
)

target_link_options(memory_stick_ro PRIVATE -nostdlib -T${PROJECT_BINARY_DIR}/linker.x)