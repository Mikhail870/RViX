// сервер файловой системы
// принимает запросы от программ
// управляет дескрипторами
#include "lib.h"
#include "VFS.h"

struct msg ipc_send;
void main(void){
  while (1) {
    ipc_send=recv();
    uint64 namesrc=ipc_send.a1;
    // проверка номера вызова
    switch (ipc_send.a5){
      //write
      case 1: 
        //проврека дескриптора
        switch (ipc_send.a0) {
          case 1:
            // перенаправление в консоль
            if (ipc_send.a4==1){
            ipc_buf_cpy(terminal,2,ipc_send.a2);// обращение не по имени ошибка !!!
            send(0,0,0,0,0,0,terminal);
            }
            break;
          case 2:
            // поток ошибок
            break;
          case 3:
            // работа с файлом (перенаправление к фс)
            break;
        }
        break;
      case 2:
        // open
        break;
    }
  }
}

// анонимные вызовы для копирования памяти
// копирует буфер IPC отправителя, в IPC буфер получателя
// dst - имя процесса отправителя
// src имя роцесса получаетля (сервреа)
// size размер собщения
  uint64 ipc_buf_cpy(uint64 dst, uint64 src, int size){
  if (size>PGSIZE)
    return -1;

  register long a0 __asm__("a0")=dst;
  register long a1 __asm__("a1")=src;
  register long a2 __asm__("a2")=size;
  register long a7 __asm__("a7")=2; // код ipc_buf_cpy()
__asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a1), "r"(a2),"r"(a7)
                         : "memory");

  return (uint64)a0;

}
