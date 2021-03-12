add_executable(memory_stick_ro)

target_sources(memory_stick_ro PRIVATE main.cpp)
target_sources(memory_stick_ro PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../crt0.s)

target_link_libraries(memory_stick_ro iplsdk::pff iplsdk::serial iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(flash_led PRIVATE -flto)

add_custom_command(
        TARGET memory_stick_ro
        POST_BUILD
        COMMAND psp-objcopy -O binary memory_stick_ro memory_stick_ro.bin
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../convert_to_tm.py --infile memory_stick_ro.bin --outfile memory_stick_ro_tm.bin
)

target_link_options(memory_stick_ro PRIVATE -nostdlib -T${CMAKE_CURRENT_SOURCE_DIR}/../linker.x)