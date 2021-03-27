OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips:allegrex)
ENTRY(_start)

STACK_SIZE = @STACK_SIZE@;

SECTIONS
{
    . = @LOAD_ADDR@;
    .text :
    {
        KEEP(*(.text.vector))
        *(.text)
    }
    .sdata : { *(.sdata) *(.sdata.*) *(.gnu.linkonce.s.*) }
    .rodata : { *(.rodata) }
    .data : { *(.data*) }
    .bss (NOLOAD):
    {
        __bss_start = .;
        *(.bss* .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    .stack (NOLOAD):
    {
        . = ALIGN(4);
        __stack_bottom = .;
        . = . + STACK_SIZE;
        . = ALIGN(4);
        __stack_top = .;
    }
}
