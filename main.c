#include "types.h"
#include "riscv.h"
#include "memlayout.h"
#include "common.h"
#include "vm.h"
#include "defs.h"
#include "kalloc.h"
extern pagetable_t kernel_pagetable;

void  main() {
printf("RViX booted !\n");
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц
printf("main adrr %p\n",main);
printf("IPC adrr %p\n",IPC);
for(;;){;}
 __asm__ __volatile__("wfi");
}
