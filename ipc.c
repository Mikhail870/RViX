/*
 Реализация IPC 
Главный механизм ядра, пересылает сообщения процесс <-> сервер
Механизм:
Пользовтельский процесс делает syscall -> syscall обртка над ipc ->
-> вызов ipc эквивалентен ecall -> обрабочик прерываний видит преывания от 
системного вызва -> проверяет его тип и адресат -> делает пересылку.

Таким образом IPC вызывается в trap_hadle.c
*/
#include "type.h"
#include "common.h"
#include "ipc.h"


IPC_call(){
struct IPC_reg IPC_data;
IPC_data.a0=current->a0;
IPC_data.a1=current->a1;
IPC_data.a2=current->a2;
IPC_data.a3=current->a3;
IPC_data.a4=current->a4;
IPC_data.a5=current->a5;
IPC_data.a6=current->a6;

 switch(current->a7){
    case 0:
    send();// реализовать
    break;
    case 1:
    recv()// реализовать
    default:
    PANIC("UNKNOW IPC CALL");
  }
    

}
