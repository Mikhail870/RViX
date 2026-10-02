#include "lib.h"

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
    // ipc_buf_cpy(name_reader,uinput,size)
    wakeup(name_reader)
  }
}

uint64 wakeup(uint64 name){
  register long a0 __asm__("a0")=name;
  register long a7 __asm__("a7")=5; // код wakeup
__asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a7)
                         : "memory");
  return (uint64)a0;
}
