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

#include "user/prg1_data.h"
#include "user/prg2_data.h"
#include "user/prg3_data.h"
#include "user/VFS_data.h"

void  main() {
printf("RViX booted !\n");
w_stvec((uint64)kerneltrap);
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц
//intr_off(); 
proc_born((uint64*)user_VFS_bin,(uint64)user_VFS_bin_len,10);// создание сервреа из бинарника
proc_born((uint64*)user_prg3_bin,(uint64)user_prg3_bin_len,3);// создание процесса из бинарника
proc_born((uint64*)user_prg2_bin,(uint64)user_prg2_bin_len,2);// создание процесса из бинарника
proc_born((uint64*)user_prg1_bin,(uint64)user_prg1_bin_len,1);// создание процесса из бинарника
w_sie(SIE_STIE);
intr_on();
//set_timer(10000);
yield();
while(1){
  asm volatile("wfi");}
}
