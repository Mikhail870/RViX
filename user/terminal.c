// сервер терминала
// выводит в юарт символы
// будет принимать символы
// аппаратно завимсим из-за мапинга адресов !
//
#include "lib.h"

struct msg ipc_send;
void main(){
  volatile char *uart=(char*)UART;
  while(1){
    ipc_send=recv();
    char* strmem=(char*)IPC_BUFF;
   /* char ch= (char)ipc_send.a1;
    if (ch=='\n'){
      *uart='\r';
      *uart='\n';
    } else {*/ 
    while(!(*strmem==0)){ 
    *uart=*strmem;
    strmem++;
    }
    }
  
}
