.set noreorder

.section .text.vector
.global _start

.ent _start
_start:
    jal main
    lui $gp, 0
    li $sp, 0x80013FF0
    li $t9, 0x80010000
    jr $t9
    nop
.end _start
.set reorder

