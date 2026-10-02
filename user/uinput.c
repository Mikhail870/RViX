#include "lib.h"
#include "servers.h"

struct msg ipc_send; 
char* strmem=(char*)IPC_BUFF;
volatile char *uart=(volatile char*)UART;
int main(){
  while (1){
    ipc_send=recv();
    char sym=0;
    int count=0;
    uint64 name_reader=ipc_send.a0;
    uint64 buff=ipc_send.a1;
    uint64 size=ipc_send.a2;
    while(sym!='\r' && count<size){ 
    if (ReadReg(LSR) & LSR_RX_READY){
      sym=ReadReg(RHR);
      WriteReg(0,sym);
      strmem[count++]=sym;
    }
    }
    if (count<size)
      strmem[count++]='\0';
    ipc_buf_cpy(name_reader,uinput,size);
    wakeup(name_reader);
    // логика чтения из UART
    // копируем читающему процессу
    // если нажата энтер копируем буфер 
    // и будим процесс
  }
}

