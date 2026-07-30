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
    //send();// реализовать
    printf("%d\n",IPC_data.a0);// котсыль для отладки
    break;
    case 1:
    //recv()// реализовать
    printf("call num 1"); // котсыль для отладки
    break;
    default:
    PANIC("UNKNOW IPC CALL");
  }
}

int send(uint64 arg1,uint64 arg7){
}
// копирует регистры a0-a7 из src в dst
void copy_reg(struct process *src, struct process *dst){
src->trapframe->a0=dst->trapframe->a0;
src->trapframe->a1=dst->trapframe->a1;
src->trapframe->a2=dst->trapframe->a2;
src->trapframe->a3=dst->trapframe->a3;
src->trapframe->a4=dst->trapframe->a4;
src->trapframe->a5=dst->trapframe->a5;
src->trapframe->a6=dst->trapframe->a6;
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
void add_que(struct process *prc, uint64 name){

}
