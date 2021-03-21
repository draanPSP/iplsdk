add_executable(memory_stick_rw)

target_sources(memory_stick_rw PRIVATE main.cpp)
target_sources(memory_stick_rw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../crt0.s)

target_link_libraries(memory_stick_rw iplsdk::fatfs iplsdk::serial iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(flash_led PRIVATE -flto)

add_custom_command(
        TARGET memory_stick_rw
        POST_BUILD
        COMMAND psp-objcopy -O binary memory_stick_rw memory_stick_rw.bin
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../convert_to_tm.py --infile memory_stick_rw.bin --outfile memory_stick_rw_tm.bin
)

target_link_options(memory_stick_rw PRIVATE -nostdlib -T${CMAKE_CURRENT_SOURCE_DIR}/../linker.x)