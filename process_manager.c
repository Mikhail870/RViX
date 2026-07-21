/*
Тут нахохдится процесс сервер упрвления процессами
Содержит:
Инициализатор процессов
Структуру процесса
Ведет таблицу процессов
Планировщик процессов
Этот код будет рабоать в U mode 
Обращаться к yield или к прочим компоненетам нужно будет через IPC
swtch и то что требует S mode в дальнейшем будет работой самого ядра.
Для обращения к ним все так же IPC
*/
// TODO добавить swtch.S и yield()

#include "types.h"
#include "common.h"
#include "memlayot.h"
#include "vm.h"
#include "process_manager.h"
#include "timer.h"

struct process proc[MAX_PROS]; // структуры под процессы
struct process *next;
struct process *current;

// функция подготавливающая регистры и таблицу страниц к переходу 
// в U modeи и осущесвтляет прыжок в uservec, вызывается в proc_born() вместо
// forkret, адрес кладется в ra, взята частично из xv6
  void prepare_uret(void){
  //взято из xv6:
  uint64 trampoline_uservec = TRAMPOLINE + (uservec - trampoline);
  w_stvec(trampoline_uservec);
  // set up trapframe values that uservec will need when
  // the process next traps into the kernel.
  p->trapframe->kernel_satp = r_satp();         // kernel page table
  p->trapframe->kernel_sp = p->kstack + PGSIZE; // process's kernel stack
  p->trapframe->kernel_trap = (uint64)usertrap;
  p->trapframe->kernel_hartid = r_tp(); // hartid for cpuid()
  // set up the registers that trampoline.S's sret will use
  // to get to user space.
  // set S Previous Privilege mode to User.
  unsigned long x = r_sstatus();
  x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
  x |= SSTATUS_SPIE; // enable interrupts in user mode
  w_sstatus(x);
  // set S Exception Program Counter to the saved user pc.
  w_sepc(p->trapframe->epc);
  uint64 satp = MAKE_SATP(p->pagetable);
  uint64 trampoline_userret = TRAMPOLINE + (userret - trampoline);
  ((void (*)(uint64))trampoline_userret)(satp);

}


// Функция, создающая процесс
// принимает адрес кода и создает из него процесс. 
// если успешно, то вернет 1
// иначе вернет код ошибки 0
 void proc_born(uint64 *binprc,uint64 size){

  struct process *pc = NULL;
  int i;
  for (i=0;i<MAX_PROS;i++) {
    if (proc[i].state==UNUSED){
      proc[i].state=RUN;// на случай если тикнет таймер и процесс не запустится, стек еще не готов
      pc=&proc[i];
      break;
    }
  }
  pc->pid=i+1; // процесс с PID 0 всегда 1. init_process(0) имеет PID 0 вручную
  if(!pc)
    PANIC("NOT FREE SLOTS FOR PROCESS");

// выделяем страницу под trapframe
  if((pc->trapframe=(struct trapframe*)kalloc())==0){
    PANIC("page for trapframe dont allocation")
    // тут добавиь освобождние процессами
  }
 
  // формируем таблицу страниц пользователського процесса
  pagetable_t npagetable;
  pagetable=uvmcreate()
  if (npagetable==0)
    PANIC("pagetable dont create");
  // маппинг trampoline на вирткальну. память
  if (mappages(npagetable, TRAMPOLINE, PGSIZE, (uint64)trampoline,
               PTE_R | PTE_X) < 0) {
    uvmfree(npagetable, 0);
  }
  // маппинг trapframe на виртуальную память 
  if (mappages(npagetable, TRAPFRAME, PGSIZE, (uint64)(p->trapframe),
               PTE_R | PTE_W) < 0) {
    uvmunmap(npagetable, TRAMPOLINE, 1, 0);
    uvmfree(npagetable, 0);
  }
  pc->pagetable=npagetable;
  
  // обнуляем контекст
  memset(&pc->context,0,sizeof(pc->context));
  pc->trapframe->epc=0;
  //стек ядра
  pc->context.sp=pc->context.sp+PAGESIZE;

  // Запись бинарника по адресу 0x0 в виртуальную память
  uint64 *memphys;
  if (size>PAGESIZE)
    PANIC(size of bin process too large !);
  if ((memphys=(uint64*)kalloc())==0)
    PANIC("dont get phys page for process !");
  memset(memphys,0,PAGESIZE);
  memmove(memphys,binprc,PAGESIZE);
  mappages(pc->pagetable, 0, PAGESIZE, (uint64)memphys, PTE_U|PTE_X|PTE_R|PTE_W);

  // прыжок в userret
  pc->context.ra=(uint64)prepare_uret
  pc->state=RUNABBLE;
}

// Создание процессов серверов, когда все заняты, то крутится как IDLE
void idle(void){
  while(1){
    printf("idle");
    asm volatile("wfi");
  }
}
struct process *init_process(int param){
  // param=0 создать базовые процессы
  // param=1 создать IDLE процесс 
  if (param==0){
    // пока что ничего
  } else if (param==1) {
  
    struct process *idle_prc=proc_born((uint64)idle) ;  
    idle_prc->pid=-1;
   // Запускаем IDLE как первый процесс заглушку
    current=idle_prc;
  // первый процесс пишется в структуру процессов как будто он уже работал
    return idle_prc;
  } else {
    PANIC("UNCORRECT PARAM IN init_process");
  }
 }

// Реализация планировщика, выбираем процессов готовый к работе и PID>0
void sheduller(void){
static int idx=0;
next=NULL;
for(int i=0; i< MAX_PROS; i++){
    int id=(idx+i+1) % MAX_PROS;
    struct process *p=&proc[id];
    if (p->state==RUNABBLE && p->pid > 0){
      next=p;
      idx=id;
      break;
    }
  }
  
  if(!next)
    //в случает отсутсвия свободных процессов вызывается swtch.S и IDLE
    PANIC("NOT RUNABBLE PROCESS");

}

// Где то тут будет вход в swtch.S
void yield(void){
  sheduller(); // Выбираем следующий процесс 
  struct process *prev=current;
  current=next;
  prev->state=RUNABBLE;
  next->state=RUN;
  asm volatile("csrrs zero, sstatus, %0"::"r"(1<<1));
  switch_context(&prev->sp,&current->sp);// меняем контекст выполнения. При запуске current всегда IDLE процесс
}


