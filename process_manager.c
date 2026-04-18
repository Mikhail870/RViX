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


struct process proc[MAX_PROS]; // структуры под процессы
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


  *--sp=(uint64) prc; // Адрес возврата, функция передаваемая в proc_born

  for (int i=0; i<12; i++)
    *--sp=0;

  pc->pid=i+1;
  pc->sp = sp;
  pc->state=RUNABBLE;

  return pc;
}

void sheduller(void){

}
