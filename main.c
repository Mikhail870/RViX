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
  //asm volatile("csrrs zero, sstatus, %0"::"r"(1<<1));
  while(1){
  printf("A");
  
  }}

void proc_B(void){
  //asm volatile("csrrs zero, sstatus, %0"::"r"(1<<1));
  while(1){
  printf("B");
}
  }

void proc_C(void){
  //asm volatile("csrrs zero, sstatus, %0"::"r"(1<<1));
  while(1){
  printf("C");
}
  }


void  main() {
printf("RViX booted !\n");
kinit(); // нарезка свободной памяти на стриницы
kvminit(); // создание таблицы страниц ядра (требует изменения в vm.c !)
kvminithart(); // включаем таблиццу страниц

set_stvec(kernelvec);// Записать адрес обработчика в stvec
init_interrupt_enable();// Разрешить голобальные прерывания
  
struct process *pA=proc_born((uint64)proc_A);
struct process *pC=proc_born((uint64)proc_C);
struct process *pB=proc_born((uint64)proc_B);
struct process *pI=init_process(1); // idle процесс 
current=pI;
set_timer(1000000);
while(1){
  asm volatile("wfi");}
}
