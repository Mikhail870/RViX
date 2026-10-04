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
#include "trap_handle.h"
#include "bootstrap.h"

void  main() {
printf("RViX booted !\n");
w_stvec((uint64)kerneltrap);
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц
//intr_off(); 

init_servers(); // инициализация серверов 

w_sie(SIE_STIE);
intr_on();
//set_timer(10000);
yield();
while(1){
  asm volatile("wfi");}
}
