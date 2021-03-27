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
    
    la $sp, __stack_top

#    jal _init
    move $gp, $0
    
    jal main
    nop

#    jal _fini
#    nop   
    
inf_loop:
    b inf_loop
    nop
.end _start
.set reorder

