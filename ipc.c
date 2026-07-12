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

int send(uint ID_dest,long arg0,long arg1,long arg2,long arg3,long arg7){
  register long a0 __asm__("a0")=arg0;
  register long a1 __asm__("a1")=arg1;
  register long a2 __asm__("a2")=arg2;
  register long a3 __asm__("a3")=arg3;
  register long a7 __asm__("a7")=arg7;
  
}

int recieve(){

}

