#include "types.h"
#include "riscv.h"
#include "memlayout.h"
#include "common.h"
#include "vm.h"
#include "defs.h"
#include "kalloc.h"
#include "timer.h"
#include "PLIC.h"
#include "process_manager.h"



void  main() {
printf("RViX booted !\n");
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц

intr_off(); 
//born_process(); создание процесса из бинарника
intr_in();

set_timer(1000000);
while(1){
  asm volatile("wfi");}
}
