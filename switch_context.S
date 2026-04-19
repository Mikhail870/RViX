# переключалка контекста выполнения

.globl switch_context

switch_context:
addi sp, sp, -14*8 # выделить на стеке ядра 13 ячеек по 8 байт под контекст
sd ra, 0*8(sp)
sd s0, 1*8(sp)
sd s1, 2*8(sp)
sd s2, 3*8(sp)
sd s3, 4*8(sp)
sd s4, 5*8(sp)
sd s5, 6*8(sp)
sd s6, 7*8(sp)
sd s7, 8*8(sp)
sd s8, 9*8(sp)
sd s9, 10*8(sp)
sd s10, 11*8(sp)
sd s11, 12*8(sp)
sd sp, (a0)
# загрузка регистров предыдущего процесса
ld sp, (a1)
ld ra, 0*8(sp)
ld s0, 1*8(sp)
ld s1, 2*8(sp)
ld s2, 3*8(sp)
ld s3, 4*8(sp)
ld s4, 5*8(sp)
ld s5, 6*8(sp)
ld s6, 7*8(sp)
ld s7, 8*8(sp)
ld s8, 9*8(sp)
ld s9, 10*8(sp)
ld s10, 11*8(sp)
ld s11, 12*8(sp)
addi sp, sp, 14*8 # Освободить выделенное место на стеке
ret 
