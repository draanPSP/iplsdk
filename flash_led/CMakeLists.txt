add_executable(flash_led)

target_sources(flash_led PRIVATE main.cpp crt0.s)
target_link_libraries(flash_led iplsdk::cache iplsdk::lowio iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(flash_led PRIVATE -flto)

add_custom_command(
        TARGET flash_led
        POST_BUILD
        COMMAND psp-objcopy -O binary flash_led flash_led.bin
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../convert_to_tm.py --infile flash_led.bin --outfile tm.bin
)

target_link_options(flash_led PRIVATE -nostartfiles -nostdlib -nolibc -T${CMAKE_CURRENT_SOURCE_DIR}/linker.x)