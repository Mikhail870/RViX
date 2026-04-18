#include "types.h"
#include "riscv.h"
#include "memlayout.h"
#include "common.h"
#include "vm.h"
#include "defs.h"
#include "kalloc.h"
#include "timer.h"
extern pagetable_t kernel_pagetable;
extern char kernelvec[];


void  main() {
printf("RViX booted !\n");
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц
printf("main adrr %p\n",main);
printf("IPC adrr %p\n",IPC);
for(int i=0;i<10;i++){
uint64 time = get_time();
printf("%d\n",time);}


asm volatile("csrw stvec, %0" : : "r"(kernelvec));
asm volatile("csrs sie, %0" : : "r"(1 << 5));
asm volatile("csrs sie, %0" : : "r"(1 << 7));
asm volatile("csrs sstatus, %0" : : "r"(1 << 1));
uint64 sstatus=0;
asm volatile("csrr %0, sstatus":"=r"(sstatus));
printf("sstatus %x\n",sstatus);
set_timer(10000);
  uint64 sp;
asm volatile("mv %0, sp" : "=r"(sp));
printf("Current SP: %p\n", sp);
for(;;){;}
 __asm__ __volatile__("wfi");
}
