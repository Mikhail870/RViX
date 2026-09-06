// сервер терминала
// выводит в юарт символы
// будет принимать символы
// аппаратно завимсим из-за мапинга адресов !
//
#include "lib.h"

struct msg ipc_send;
void main(){
  volatile char *uart=(char*)UART;  
  *uart='c';
  while(1){
    ipc_send=recv();

  }
  
}
