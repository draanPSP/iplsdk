project(samples VERSION 1.0 LANGUAGES C CXX ASM)

set(LOAD_ADDR 0x040F0000)
set(STACK_SIZE 0x2000)

get_target_property(LINKER_SCRIPT iplsdk::linker_script INTERFACE_SOURCES)

configure_file(
    "${LINKER_SCRIPT}"
    "${CMAKE_CURRENT_BINARY_DIR}/linker.x"
)

add_subdirectory(flash_led)
add_subdirectory(controller)
add_subdirectory(hp_remote_serial)
add_subdirectory(kirk)
add_subdirectory(memory_stick_ro)
add_subdirectory(memory_stick_rw)
add_subdirectory(nand)
