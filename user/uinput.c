#include "lib.h"
#include "servers.h"

struct msg ipc_send; 
char* buff=(char*)IPC_BUFF;
int main(){
  while (1){
    ipc_send=recv();
    uint64 name_reader=ipc_send.a0;
    uint64 buff=ipc_send.a1;
    uint64 size=ipc_send.a2;
    // логика чтения из UART
    // копируем читающему процессу
    // если нажата энтер копируем буфер 
    // и будим процесс
    ipc_buf_cpy(name_reader,uinput,size)
    wakeup(name_reader)
  }
}

