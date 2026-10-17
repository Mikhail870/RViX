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

int init_servers(void);


int main(){
init_servers();
}




struct PCB ptable[MAX_PROC]={0};

// инициализация PCB для запущенных серверов
// вызывается при первом запуске UPM
// заполняет PCB прооцессов
// если что то пошло не так возвращает -1, иначе 0
//
int init_servers(){
int servers[]={VFS,terminal,shell,uinput};
int size=sizeof(servers)/sizeof(servers[0]);

for(int i=0;i<size;i++){
    if(ptable[i].pid==0){
      ptable[i].name=servers[i];
      ptable[i].pid=i+1;
  } else {
    return -1;
  }
}
  return 0;
}

// обработка вызова fork()
// создает точную копию потока вызвавшего процесса
// прописывает в TCB в ядре и PCB в UPM
// возвращает PID родителю и 0 потомку
int fork(uint64 name){
  // создает запись о процессе в PCB 
  // возвращает имя процесса (id)
  // регистриуеи процесс в TCB
  // копирует страницы от родителя потомку
  // будит процесс
  // uint64 name = add_PCB();
  // add_TCB(cname); // имя потомка в TCB
  copy_page_table(uint64 pname,uint64 cname);
  
}
