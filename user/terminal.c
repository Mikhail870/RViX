// сервер терминала
// выводит в юарт символы
// будет принимать символы
// аппаратно завимсим из-за мапинга адресов !
//
#include "lib.h"

struct msg ipc_send;
void main(){
  volatile char *uart=(volatile char*)UART;
  memset((char*)IPC_BUFF,0,PGSIZE);

  while(1){
    ipc_send=recv();
    char* strmem=(char*)IPC_BUFF;
    *uart=ipc_send.a0;
    while (*strmem!=0){
      *uart=*strmem++; 
    }
  } 
}
