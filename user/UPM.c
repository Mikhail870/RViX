#include "lib.h"
#include "servers.h"

#define MAX_PROX 8
// сервер процессов
// при инициализации системы и запуске серверов назначает и PID и заполняет их PCB
// отвечает на создание новых процессов (fork+exec)
struct PCB {
  int pid;
  int killed;
  int xstate;
  // добавить массив файлов  обращением по дескриптору o!
};

struct PCB ptable[MAX_PROX];


// инициализация PCB для запущенных серверов
// вызывается при первом запуске UPM
// заполняет PCB прооцессов

