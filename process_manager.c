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
#include "riscv.h"
#include "common.h"
#include "memlayout.h"
#include "vm.h"
#include "process_manager.h"
#include "timer.h"
#include "trap_handle.h"


struct process proc[MAX_PROS]; // структуры под процессы
struct process *next;
struct process  * volatile current = NULL;


extern char trampoline[],uservec[],userret[];


// функция подготавливающая регистры и таблицу страниц к переходу 
// в U modeи и осущесвтляет прыжок в uservec, вызывается в proc_born() вместо
// forkret, адрес кладется в ra, взята частично из xv6
  void prepare_uret(void){
  //взято из xv6:
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
  uint64 satp = MAKE_SATP(current->pagetable);
  uint64 trampoline_userret = TRAMPOLINE + (userret - trampoline);
  ((void (*)(uint64))trampoline_userret)(satp);

}


// Функция, создающая процесс
// принимает адрес кода и создает из него процесс. 
// если успешно, то вернет 1
// иначе вернет код ошибки 0
 void proc_born(uint64 *binprc,uint64 size, uint64 name,int is_io_server){

  struct process *pc = NULL;
  int i;
  for (i=0;i<MAX_PROS;i++) {
    if (proc[i].state==UNUSED){
      proc[i].state=RUN;// на случай если тикнет таймер и процесс не запустится, стек еще не готов
      pc=&proc[i];
      break;
    }
  }
  if(!pc)
    PANIC("NOT FREE SLOTS FOR PROCESS");
  pc->pid=i+1; // процесс с PID 0 всегда 1. init_process(0) имеет PID 0 вручную

// выделяем страницу под trapframe
  if((pc->trapframe=(struct trapframe*)kalloc())==0){
    PANIC("page for trapframe dont allocation");
    // тут добавиь освобождние процессами
  }
// выделяем страницу под ipc
  if((pc->ipc_page=(uint64*)kalloc())==0){
    PANIC("page for trapframe dont allocation");
    // тут добавиь освобождние процессами
  }
 
  // формируем таблицу страниц пользователського процесса
  pagetable_t npagetable;
  npagetable=uvmcreate();
  if (npagetable==0)
    PANIC("pagetable dont create");
  // маппинг trampoline на вирткальну. память
  if (mappages(npagetable, TRAMPOLINE, PGSIZE, (uint64)trampoline,
               PTE_R | PTE_X) < 0) {
    uvmfree(npagetable, 0);
  }
  // маппинг trapframe на виртуальную память 
  if (mappages(npagetable, TRAPFRAME, PGSIZE, (uint64)(pc->trapframe),
               PTE_R | PTE_W) < 0) {
    uvmunmap(npagetable, TRAMPOLINE, 1, 0);
    uvmfree(npagetable, 0);
  }
  // маппинг буфера IPC на виртуальную память
  if (mappages(npagetable, IPC, PGSIZE, (uint64)(pc->ipc_page),
               PTE_R | PTE_W | PTE_U) < 0) {
    uvmunmap(npagetable, TRAMPOLINE, 1, 0);
    uvmfree(npagetable, 0);
  }
  // если это севрер ввода вывода, то мапим регистры юарт
  if (is_io_server==1){

  
  if (mappages(npagetable, IPC, PGSIZE, (uint64)(pc->ipc_page),
               PTE_R | PTE_W | PTE_U) < 0) {
    uvmunmap(npagetable, TRAMPOLINE, 1, 0);
    uvmfree(npagetable, 0);
  }
  pc->pagetable=npagetable;

  // выделяем физическую страницу и мапим на 



  // обнуляем контекст
  memset(&pc->context,0,sizeof(pc->context));
  // ставим счетчик команд юзера на виртуальный адрес 0x0
  pc->trapframe->epc=0;

  // подготавливаем страницу для блока ipc
  if((pc->ipc_data=(struct ipc_data*)kalloc())==0)
    PANIC("page for ipc dont allocation");// освободить процесс
  // обнуляем очередь que для ipc
  memset(pc->ipc_data->que,0,sizeof(pc->ipc_data->que));
  pc->ipc_data->name=name;

  //стек ядра
  uint64 *kstck;
  if ((kstck=(uint64*)kalloc())==0)
    PANIC("kernel stack dont create");
  pc->context.sp=(uint64)((uint64)kstck+PGSIZE);
  pc->kstack=(uint64)kstck;
  pc->trapframe->sp=PGSIZE; // стек юзера
  // Запись бинарника по адресу 0x0 в виртуальную память
  uint64 *memphys;
  if (size>PGSIZE)
    PANIC("size of bin process too large !");
  if ((memphys=(uint64*)kalloc())==0)
    PANIC("dont get phys page for process !");
  memset(memphys,0,PGSIZE);
  memmove(memphys,binprc,size);
  mappages(pc->pagetable, 0, PGSIZE, (uint64)memphys, PTE_U|PTE_X|PTE_R|PTE_W);
  // прыжок в userret
  pc->context.ra=(uint64)prepare_uret;
  pc->state=RUNABBLE;
  printf("prosess PID %d OK\n",pc->pid);
}


// формирует структуру проесса заглушку для записи в нее регистров при
// самом первом вызове yield(). В нее зааписываются регистры main()
// В дальнейшем proc[0] будет освобождаться для других процессов
struct process *init_process(int param){
  int prm=param; // зарезеривровано !
  struct process *idle_prc=&proc[0];
  idle_prc->pid=-1;
  idle_prc->state=RUN;
  return idle_prc;
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
    //в случает отсутсвия свободных процессов выполянется поседний процесс
    next=current;
    //PANIC("NOT RUNABBLE PROCESS");

}

// Где то тут будет вход в swtch.S
void yield(void){
  sheduller(); // Выбираем следующий процесс 
  struct process *prev=current;
  if (prev==NULL){
    // выполнится если yield() вызвана впервые и сохранит регистры main в dummy
   static struct context dummy_main; // костыль со static
    current=next; // теперь current не NULL
    current->state=RUN;
    switch_context(&dummy_main,&current->context);
    return; // больше не вернемся в эту точку
  }
  if (current->ipc_data->is_wait_msg==1){
  current=next;
  prev->state=SLEEP;
  current->state=RUN;
  } else {
  current=next;
  prev->state=RUNABBLE;
  current->state=RUN;
  }
 // asm volatile("csrrs zero, sstatus, %0"::"r"(1<<1));
  switch_context(&prev->context,&current->context);// меняем контекст выполнения. При запуске current всегда IDLE процесс
  //switch_context(struct context old,struct context new);
}

