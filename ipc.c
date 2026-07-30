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
IPC_data.a0=current->trapframe->a0;
IPC_data.a1=current->trapframe->a1;
IPC_data.a2=current->trapframe->a2;
IPC_data.a3=current->trapframe->a3;
IPC_data.a4=current->trapframe->a4;
IPC_data.a5=current->trapframe->a5;
IPC_data.a6=current->trapframe->a6;

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
