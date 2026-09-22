#include "lib.h"
// функция делает ipc вызов, отправляющий
// a0-a3 полезное сообщение (параметры систменого вызова)
// a4 - опции систменого вызова (для каждого немерация своя)
// a5 - код POSIX вызова (даже в 32 битный регистр уместится свыше 4 млрд)
// a6 - имя получаетеля
// в регистр a7 автоматически подставляется код вызова
// функция возвращает код в a0;
int send(long arg0,long arg1,long arg2,
         long arg3, long arg4, long arg5,long arg6){

  register long a0 __asm__("a0")=arg0;
  register long a1 __asm__("a1")=arg1;
  register long a2 __asm__("a2")=arg2;
  register long a3 __asm__("a3")=arg3;
  register long a4 __asm__("a4")=arg4;
  register long a5 __asm__("a5")=arg5;
  register long a6 __asm__("a6")=arg6;
  register long a7 __asm__("a7")=0; // код send()

  __asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a1), "r"(a2), "r"(a3), "r"(a4),
     "r"(a5), "r"(a6),"r"(a7)
                         : "memory");

  return (int)a0;
}

// функция делает ipc вызов, отправляющий
// a7 - код вызова, подставляется автоматически
// функция возвращает a0-a5 - полезное сообщение
struct msg recv(){
  struct msg ipc_msg;

  register long a0 __asm__("a0");
  register long a1 __asm__("a1");
  register long a2 __asm__("a2");
  register long a3 __asm__("a3");
  register long a4 __asm__("a4");
  register long a5 __asm__("a5");
  register long a7 __asm__("a7")=1; // код recv()

  __asm__ __volatile__ ("ecall"
    : "=r"(a0),"=r"(a1),"=r"(a2),"=r"(a3),"=r"(a4),"=r"(a5)
    : "r"(a7)
    : "memory");
  
  ipc_msg.a0=a0;
  ipc_msg.a1=a1;
  ipc_msg.a2=a2;
  ipc_msg.a3=a3;
  ipc_msg.a4=a4;
  ipc_msg.a5=a5;

  return ipc_msg;
}

void printk(long x){
  register long a0 __asm__("a0")=x;
  register long a7 __asm__("a7")=3;

  __asm__ __volatile__ ("ecall"
  :
  :"r"(a0),"r"(a7)
  : "memory");
}

// системные вызовы
// write иммеет номер 1
ssize_t write(int fd, const void *buf, size_t count){
  // вызов send для VFS->terminal
  // при маленьких сообщениях коипрование из буфера отправителя в буфер получателя
  // в ядре маппинг ipc страницы отправителя на ipc страницу получаетеля
  // получение серврером (получателем)
  if (fd != 2 && fd != 1){
    return -1;  
  }
  // номер write 1
  // опция копирование из IPC_BUFF в IPC_BUFF 1
  // опция копирование из произволного места памяти в произволное 2
  // опция сообщение больше 4 кб, то пермапить страницы получаетелю 3 
  // опция резерв номера до 2^64

  // проверка размера пердаваемого буфера
  if (count<=PGSIZE){
    // проверка адреса буфера
    if (buf==IPC_BUFF){
      send(fd,(long)buf,count,0,1,1,VFS);
    } else {
      send(fd,(long)buf,count,0,2,1,VFS);
    }
  } else {
     send(fd,(long)buf,count,0,3,1,VFS); // не реализовано (заглушка)
  }
  return count;
}

// функции Си

void putchar(char ch){
  char* strmem=(char*)IPC_BUFF;
  strmem[0]=ch;
  strmem[1]=0;
  write(1,(long*)IPC_BUFF,1);
}

void puts(const char *str){
  char* strmem=(char*)IPC_BUFF;
  int position=0;
  while(*str && position<(PGSIZE-1)){
     strmem[position]=*str;
      str++;
      position++;
  }
  if (position>0)
    write(1,(long*)IPC_BUFF,position);
}
