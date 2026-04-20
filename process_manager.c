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
#include "process_manager.h"
#include "timer.h"

struct process proc[MAX_PROS]; // структуры под процессы
struct process *next;
struct process *current;


// Функция, создающая процесс
// принимает адрес кода и создает из него процесс. 
// если успешно, то вернет 1
// иначе вернет код ошибки 0
struct process *proc_born(uint64 prc){

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

  uint64 *sp = (uint64*)( (char*)pc->stack+sizeof(pc->stack));
  sp = (uint64*)((uint64)sp & ~0xFULL);
  
  for (int i=0; i<13; i++)
    *--sp=0;

  *--sp=(uint64) prc; // Адрес возврата, функция передаваемая в proc_born
   
  pc->pid=i+1; // процесс с PID 0 всегда 1. init_process(0) имеет PID 0 вручную
  pc->sp = sp;
  pc->state=RUNABBLE;

  return pc;
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


