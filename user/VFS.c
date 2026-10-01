// сервер файловой системы
// принимает запросы от программ
// управляет дескрипторами
#include "lib.h"
#include "VFS.h"

struct msg ipc_send;
void main(void){
  while (1) {
    ipc_send=recv();
    uint64 namesrc=ipc_send.a3;
    // проверка номера вызова
    switch (ipc_send.a5){
      //write
      case 1: 
        //проврека дескриптора
        switch (ipc_send.a0) {
          case 1:
            // перенаправление в консоль
            // проврека параметра буфера a4
            // если 1 то коирование из IPC буфера в IPC буфрер терминала
            // если 2 то коирование через copyin в IPC буфер терминала
            if (ipc_send.a4==1){
            ipc_buf_cpy(terminal,namesrc,ipc_send.a2);// обращение не по имени ошибка !!!
            send(0,0,0,0,0,0,terminal);
            } else {
              buf2ipc_cpy(terminal,namesrc,ipc_send.a1,ipc_send.a2);
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
      //read
      case 2:
        // отправител засыпает
         sleep(namesrc);
        // вызов сервера ввода
        // send(namesrc,ipc_send.a2,0,0,0,0,uinput);
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

// копирует буфер отправителя в IPC буфер получателя
// делает вызов номер 3
// ядро принимает его и копирует через copyin данные в IPС буфер получателя
// dst - имя процесса получателя
// src - имя процесса отправителя
// bfadr_src - виртуальный адрес буфера отправителя
// size - размер собщения
uint64 buf2ipc_cpy(uint64 dst,uint64 src,uint64 bfadr_src,int size){
  if (size>PGSIZE)
    return -1;

  register long a0 __asm__("a0")=dst;
  register long a1 __asm__("a1")=bfadr_src;
  register long a2 __asm__("a2")=size;
  register long a3 __asm__("a3")=src;
  register long a7 __asm__("a7")=3; // код ipc_buf_cpy()
__asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a1), "r"(a2),"r"(a3),"r"(a7)
                         : "memory");

  return (uint64)a0;

}

//  усыпляет процесс name
// делает вызов номер 4 
uint64 sleep(uint64 name){
  register long a0 __asm__("a0")=name;
  register long a7 __asm__("a7")=4; // код sleep()
__asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a7)
                         : "memory");
  return (uint64)a0;
}
