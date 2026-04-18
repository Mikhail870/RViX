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

extern uint64 kernelvec[];

void proc_A(void){
  printf("A");
}

void  main() {
printf("RViX booted !\n");
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц

set_stvec(kernelvec);// Записать адрес обработчика в stvec
init_interrupt_enable();// Разрешить голобальные прерывания
  
struct procces *proc_A=proc_born((uint64)proc_A);
  


set_timer(10000);
for(;;){;}
 __asm__ __volatile__("wfi");
}
