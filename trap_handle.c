#include "types.h"
#include "memlayout.h"
#include "common.h"
#include "timer.h" // потом убоать, будет вызываться через IPC
#include "process_manager.h"
#include "riscv.h"

extern char trampoline[], uservec[];
/*
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
    printf("timer from s mode\n");
    yield();// Сделать вызов через IPC
    break;
    default: 
    PANIC("UNKNOW INTERUPTION");
  }
}
uint64 usertrap(void){
  if ((r_sstatus() & SSTATUS_SPP) != 0)
    PANIC("dont from U mode");
  w_stvec((uint64)kerneltrap);
  current->trapframe->epc=r_sepc();


  // сделать обработку прервыний и исключений через case
  // Добавить обработку системных вызовов !
  uint64 scause=r_scause() & 0xFF; 
  switch (scause){
    case 2:
    PANIC("ILLEGAL INSTRUCTION IN U MODE");
   // убить процесс
    break;
    case 5:
    //таймер
    printf("from user !\n");
    set_timer(1000000);
    yield();
    break;
    case 8:
    //syscall();
    
    break;
    case 12:
    PANIC("PAGEFAULT IN U MODE");
    // убить процесс
    break;
    default:
    PANIC("UNKNOW INTERUPTION FROM USER MODE");
  }
  

    // Логика prepare_return() из xv6 (хардкод)
  intr_off();
  // send syscalls, interrupts, and exceptions to uservec in trampoline.S
  uint64 trampoline_uservec = TRAMPOLINE + (uservec - trampoline);
  w_stvec(trampoline_uservec);
  // set up trapframe values that uservec will need when
  // the process next traps into the kernel.
  current->trapframe->kernel_satp = r_satp();         // kernel page table
  current->trapframe->kernel_sp = current->kstack + PGSIZE; // process's kernel stack
  current->trapframe->kernel_trap = (uint64)usertrap;
  current->trapframe->kernel_hartid = r_tp(); // hartid for cpuid()
  // set up the registers that trampoline.S's sret will use
  // to get to user space.
  // set S Previous Privilege mode to User.
  unsigned long x = r_sstatus();
  x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
  x |= SSTATUS_SPIE; // enable interrupts in user mode
  w_sstatus(x);
  // set S Exception Program Counter to the saved user pc.
  w_sepc(current->trapframe->epc);
  uint64 satp= MAKE_SATP(current->pagetable);
  set_timer(1000000);
    return satp;
  
}
