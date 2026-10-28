#include "lib.h"
#include "servers.h"

#define MAX_PROC 8
// сервер процессов
// при инициализации системы и запуске серверов назначает и PID и заполняет их PCB
// отвечает на создание новых процессов (fork+exec)
struct PCB {
  int pid;
  int name; // ядерный id
  int killed;
  int xstate;
  // добавить массив файлов  обращением по дескриптору o!
};
static uint64 nextpid=0;
static uint64 nextname=0;

int init_servers(void);
uint64 add_PCB(void);
uint64 pfork(uint64);
int find_PID(uint64);

int main(){
init_servers();

  while (1) {
    struct msg mess=recv();
    uint64 num_call=mess.a5;
    uint64 caller_name=mess.a3;
      switch (num_call) {
      // fork()
      case 3:
      // вызвать pfork()
      uint64 cn=pfork(caller_name); // копия памяти того, кто вызвал форк, добавляет новый процесс в PCB и TCB
      // забрать имя копии
      int cp=find_PID(cn); // имя дочернего процесса принимает, pid возвращает
      // записать в ppid копии имя родителя
      // родителю по имени вернуть pid копирует
      send(cp,0,0,0,0,0,caller_name);
      send(0,0,0,0,0,0,cn);

      // копии по имени вернуть o 
      break;
      default:
      break;
      }
  }
}




struct PCB ptable[MAX_PROC]={0};

// инициализация PCB для запущенных серверов
// вызывается при первом запуске UPM
// заполняет PCB прооцессов
// если что то пошло не так возвращает -1, иначе 0
//
int init_servers(){
int servers[]={VFS,terminal,shell,uinput,UPM};// имена серверов строго от меньшего к большему !
int size=sizeof(servers)/sizeof(servers[0]);

for(int i=0;i<size;i++){
    if(ptable[i].pid==0){
      ptable[i].name=servers[i];
      ptable[i].pid=i+1;
      nextpid=i+2;
      nextname=servers[i]+1;
  } else {
    return -1;
  }
}
  return 0;
}

// ищет пустые слоты в PCB
// инициализирует их для новых процессов
// возвращает ядерное имя нового процесса (id)
uint64 add_PCB(){
  for(int i=0;i<MAX_PROC;i++){
    if(ptable[i].pid==0){
      ptable[i].pid=nextpid;
      ptable[i].name=nextname;
      nextpid++;
      nextname++;
      return ptable[i].name;
    } 
    }
  return 0;
  }

// обработка вызова fork()
// создает точную копию потока вызвавшего процесса
// прописывает в TCB в ядре и PCB в UPM
// возвращает PID родителю и 0 потомку
// name - имя процесса родителя
// cpid - pid потомка
uint64 pfork(uint64 pname){
  // создает запись о процессе в PCB 
  // возвращает имя процесса (id)
  // регистриуеи процесс в TCB
  // копирует страницы от родителя потомку
  // будит процесс
   uint64 cname=add_PCB();
   add_TCB(cname); // имя потомка в TCB
   copy_page_table(pname, cname);
   wakeup(cname);
  // вернкуть pid 0 потомку
  // вернуть pid потомка родителю
 return cname; 

}

// возыращает PID процессапо его имени
// name - имя процесса, PID которого ищется
int find_PID(uint64 name){
  for(int i=0;i<MAX_PROC;i++){
    if(ptable[i].name==name){
      return ptable[i].pid;} else {
      return -1; // pid не найден
    }
  }
}
