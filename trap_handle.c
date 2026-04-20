#include "common.h"
#include "timer.h" // потом убоать, будет вызываться через IPC
#include "process_manager.h"/*
Менеджер процессов и таймер используется тут, чтобы при обработке прерывания таймера
прыгнуть в yield() и сменить контекст выполнения. Затем этот хидер будет убран
и обращение к yield() будет через IPC

*/

// Вызывается в Kernelvec, адрес которого лежит в stvec
// При возникновении прерывания регистры сохранятся и мы прыгнем в kerneltrap
// Тут будет решатся что за прерывание и что с ним делать
   void kerneltrap(void){
  uint64 scause=0; // номер причины прерывания
  asm volatile ("csrr %0, scause ":"=r"(scause));
  scause= scause & 0xFF;


  switch (scause) {
    case 5:
    set_timer(1000000);
    yield();// Сделать вызов через IPC
    break;
    default: 
    PANIC("UNKNOW INTERUPTION");
  }
}

