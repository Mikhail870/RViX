#include "lib.h"

struct msg ipc_send; 
char* buff=(char*)IPC_BUFF;
int main(){
  while (1){
    ipc_send=recv();
    uint64 name_reader=ipc_send.a0;
    uint64 name_reader=ipc_send.a2;
    // логика чтения из UART
    // копируем читающему процессу
    // если бефер не пуст то будим процесс
    // ipc_buf_cpy(name_reader,uinput,size)
    // wakeup(name_reader)
  }
}
