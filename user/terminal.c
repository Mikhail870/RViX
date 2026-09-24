// сервер терминала
// выводит в юарт символы
// будет принимать символы
// аппаратно завимсим из-за мапинга адресов !
//
#include "lib.h"

struct msg ipc_send;
void main(){
  volatile char *uart=(char*)UART;

  char* strmem=(char*)IPC_BUFF;
  strmem[0]='a';
  strmem[1]=0;

  while(1){
    //ipc_send=recv();
    char* strmem=(char*)IPC_BUFF;
    while(*strmem!=0){ 
    *uart=*strmem;
    strmem++;
      if (*strmem==0)
        break;
    }
    }
  
}
