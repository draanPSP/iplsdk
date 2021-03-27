add_executable(controller)

target_sources(controller PRIVATE main.cpp)

target_link_libraries(controller PRIVATE iplsdk::crt iplsdk::ctrl iplsdk::lowio iplsdk::syscon)

# Enable Link Time Optimization - doesn't seem to be required
# target_compile_options(controller PRIVATE -flto)

add_custom_command(
    TARGET controller
    POST_BUILD
    COMMAND psp-objcopy -O binary controller controller.bin
    COMMAND python3 ${PROJECT_SOURCE_DIR}/convert_to_tm.py --infile controller.bin --outfile controller_tm.bin
)

target_link_options(controller PRIVATE -nostdlib -T${PROJECT_BINARY_DIR}/linker.x)