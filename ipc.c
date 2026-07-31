/*
 Реализация IPC 
Главный механизм ядра, пересылает сообщения процесс <-> сервер
Механизм:
Пользовтельский процесс делает syscall -> syscall обртка над ipc ->
-> вызов ipc эквивалентен ecall -> обрабочик прерываний видит преывания от 
системного вызва -> проверяет его тип и адресат -> делает пересылку.

Таким образом IPC вызывается в trap_hadle.c
*/
#include "types.h"
#include "common.h"
#include "process_manager.h" // для видимости current
#include "ipc.h"


void IPC_call(void){
struct IPC_reg IPC_data;

 switch(current->trapframe->a7){
    case 0:
    //send();
    break;
    case 1:
    //recv();
    printf("call num 1"); // котсыль для отладки
    break;
    default:
    PANIC("UNKNOW IPC CALL");
  }
}

void send(uint64 name, struct process prc){
  struct process *dst=find_name_process(name);
  if (dst->state==SLEEP){
    copy_reg(current,dst);
    runable(dst);
    return;
  } else {
    add_que(dst,name);
    sleep(current);
    return;
  }
}

void recv(void){
  uint64 name;
  struct process *src;
  if ((name=extract_que(current))!=0){
    src=find_name_process(name);
    copy_reg(src,current);
    runable(src);
    return;
  } else {
    sleep(current);
    return;
  }
}

// копирует регистры a0-a7 из src в dst
void copy_reg(struct process *src, struct process *dst){
dst->trapframe->a0=src->trapframe->a0;
dst->trapframe->a1=src->trapframe->a1;
dst->trapframe->a2=src->trapframe->a2;
dst->trapframe->a3=src->trapframe->a3;
dst->trapframe->a4=src->trapframe->a4;
dst->trapframe->a5=src->trapframe->a5;
dst->trapframe->a6=src->trapframe->a6;
}

// ищет процесс по имени кому отправляем сообщение
// возвращает указатель на структуру этого процесса, если процесса нет
// то возвращает NULL
struct process* find_name_process(uint64 name){
struct process *findness_proc;
for (int i=0;i<MAX_PROS;i++){
    findness_proc=&proc[i];
    if (findness_proc->ipc_data->name==name)
      return findness_proc;
  }
  return NULL;
}
// усыпляет процесс
void sleep(struct process *prc){
  prc->state=SLEEP;
}
// утсанавливает процесс в готовность
void runable(struct process* prc){
  prc->state=RUNABBLE;
}
// функция добавляет в очередь процесса получателя процесс отправитель
// add_que(кому ставим в очеред, кого ставим в очеред);
// если очередь переполнена, вызываем панику, в идеале вохвращать код ошибки
void add_que(struct process *prc, uint64 name){
  for(int i=0;i<MAX_PROS;i++){
    if (prc->ipc_data->que[i]==0){
      prc->ipc_data->que[i]=name;
      return;
      }
    }
  PANIC("ipc que is overflow");
  }
// функция берет первое имя из очереди и возвращает имя ожидающего процесса
// если учередь пуста возвращет 0
// найденное имя передается в findness_proc(), если 0, то уводим процесс в SLEEP
// extract_que(у кого проверяем очередь);
uint64 extract_que(struct process *prc){
  uint64 tmp_name;
  for(int i=0;i<MAX_PROS;i++){
    if(prc->ipc_data->que[i]!=0){
      tmp_name=prc->ipc_data->que[i];
      prc->ipc_data->que[i]=0;
      return tmp_name;
    }
  }
  return 0;
}

