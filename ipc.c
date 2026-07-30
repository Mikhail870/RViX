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
#include "ipc.h"
#include "process_manager.h" // для видимости current


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
src->a0=current->trapframe->a0;
src->a1=current->trapframe->a1;
src->a2=current->trapframe->a2;
src->a3=current->trapframe->a3;
src->a4=current->trapframe->a4;
src->a5=current->trapframe->a5;
src->a6=current->trapframe->a6;
}
