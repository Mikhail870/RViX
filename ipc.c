/*
 Реализация IPC 
Главный механизм ядра, пересылает сообщения процесс <-> сервер
Механизм:
Пользовтельский процесс делает syscall -> syscall обртка над ipc ->
-> вызов ipc эквивалентен ecall -> обрабочик прерываний видит преывания от 
системного вызва -> проверяет его тип и адресат -> делает пересылку.

Таким образом IPC вызывается в trap_hadle.c
*/
#include "types.h"
#include "common.h"
#include "process_manager.h" // для видимости current
#include "ipc.h"


void IPC_call(void){ // возвращет структуру для сохранения a0-a7

 switch(current->trapframe->a7){
    case 0:
    send();
    printf("call num 0\n"); // котсыль для отладки
    break;
    case 1:
    recv();
    printf("call num 1\n"); // котсыль для отладки
    break;
    case 3:
    printf("%d\n",current->trapframe->a0);
    break;
    default:
    PANIC("UNKNOW IPC CALL");
  }
}

// функция не принимает аргументов, возвращет структуру (костыль для сохранения регситров)
// send() берет имя процесса получателя из trapframe отправителя get_dst_name()
// затем полученное имя передается find_name_process() возвращающей указатель на trapframe получателя
// провеояется есть ли структура процесса
// если процесс получатель SLEEP, то копируем сообщение в его trapframe, будим получателя и выходим из трапа
// если получаетль не SLEEP, то добавляем отправителя в очередь получателя add_que()
// переводим отправителя в состояние SLEEP и указываем флаг is_wait_msg=1, чтобы планировщик не менял статус
// выходим в трап 
void send(void){
  uint64 name=get_dst_name();
  struct process *dst=find_name_process(name);
  printf("name process find in send %d\n",name);
  printf("state process find in send %d\n",dst->state);
  if (dst==NULL){
    PANIC("name dst not found");
  }
  if (dst->state==SLEEP){
    printf("is sleep: OK\n");
    copy_reg(current,dst);
    dst->ipc_data->is_wait_msg=0;
    printf("is copy_reg: OK\n");
    runable(dst);
    printf("is runable dst: OK\n");
  } else {
    add_que(dst,current->ipc_data->name);
    current->ipc_data->is_wait_msg=1;
    sleep(current);
  }
}
// функция не принимает аргументов, возвращает структуру сообщения
// recv() проверяет очередь получателя
// если очередь пуста (extract_que()==0), то получатель переходит в SLEEP, затем выходит из трапа
// если в очереди есть процесс отправитель, то берем его имя (возвращает extract_que())
// находим структуру отправителя find_name_process()
// проверяем существет ли процесс отправитель
// переводим отправителя в RUNABBLE выходим из трапа
void recv(void){
  uint64 name;
  struct process *src;
  if ((name=extract_que(current))!=0){
    src=find_name_process(name);
    if (src==NULL){
      PANIC("name source not found");
    }
    copy_reg(src,current);
    runable(src);
    src->ipc_data->is_wait_msg=0;
    printf("status send is %d\n",src->state);
  } else {
    sleep(current);
    current->ipc_data->is_wait_msg=1;
    printf("name of recv process is %d\n",current->ipc_data->name);
    printf("state of recv process is %d\n",current->state);
    printf("state sleep is %d\n",SLEEP);
  }
}



// копирует регистры a0-a7 из src в dst
void copy_reg(struct process *src, struct process *dst){
dst->trapframe->a0=src->trapframe->a0;
dst->trapframe->a1=src->trapframe->a1;
dst->trapframe->a2=src->trapframe->a2;
dst->trapframe->a3=src->trapframe->a3;
dst->trapframe->a4=src->trapframe->a4;
dst->trapframe->a5=src->trapframe->a5;
//dst->trapframe->a6=src->trapframe->a6;
}

// ищет процесс по имени кому отправляем сообщение
// возвращает указатель на структуру этого процесса, если процесса нет
// то возвращает NULL
struct process* find_name_process(uint64 name){
struct process *findness_proc;
for (int i=0;i<MAX_PROS;i++){
    findness_proc=&proc[i];
    if (findness_proc->ipc_data->name==name)
      return findness_proc;
  }
  return NULL;
}
// усыпляет процесс
void sleep(struct process *prc){
  prc->state=SLEEP;
}
// утсанавливает процесс в готовность
void runable(struct process* prc){
  prc->state=RUNABBLE;
}
// функция добавляет в очередь процесса получателя процесс отправитель
// add_que(кому ставим в очеред, кого ставим в очеред);
// если очередь переполнена, вызываем панику, в идеале вохвращать код ошибки
void add_que(struct process *prc, uint64 name){
  for(int i=0;i<MAX_PROS;i++){
    if (prc->ipc_data->que[i]==0){
      prc->ipc_data->que[i]=name;
      return;
      }
    }
  PANIC("ipc que is overflow");
  }
// функция берет первое имя из очереди и возвращает имя ожидающего процесса
// если учередь пуста возвращет 0
// найденное имя передается в findness_proc(), если 0, то уводим процесс в SLEEP
// extract_que(у кого проверяем очередь);
uint64 extract_que(struct process *prc){
  uint64 tmp_name;
  for(int i=0;i<MAX_PROS;i++){
    if(prc->ipc_data->que[i]!=0){
      tmp_name=prc->ipc_data->que[i];
      prc->ipc_data->que[i]=0;
      return tmp_name;
    }
  }
  return 0;
}

// функция возвращает имя кому предназначено сообщение 
// имя читает из регистра a6
uint64 get_dst_name(void){
  uint64 name_dst=current->trapframe->a6;
  return name_dst;
}

// функция возвращает имя текущего процесса 
// имя читает из current->ipc_data->name
uint64 get_current_name(void){
  uint64 name_current=current->ipc_data->name;
  return name_current;
}


