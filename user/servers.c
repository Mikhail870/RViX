#include "lib.h"


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


uint64 wakeup(uint64 name){
  register long a0 __asm__("a0")=name;
  register long a7 __asm__("a7")=5; // код wakeup
__asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a7)
                         : "memory");
  return (uint64)a0;
}
