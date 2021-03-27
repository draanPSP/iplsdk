.set noreorder

.section .text.vector
.global _start

.ent _start
_start:
# Clear bss
    la $a0, __bss_start
    la $a1, __bss_end
clear_bss_loop:
    sltu $a2, $a0, $a1
    addiu $a0, $a0, 4
    bnel $a2, $0, clear_bss_loop
    sw $0, -4($a0)

    li $sp, 0x040e0000
    jal main
    lui $gp, 0
    
    li $sp, 0x80013FF0
    li $t9, 0x80010000
    jr $t9
    nop
.end _start
.set reorder

