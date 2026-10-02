// сервер файловой системы
// принимает запросы от программ
// управляет дескрипторами
#include "lib.h"
#include "servers.h"

struct msg ipc_send;
void main(void){
  while (1) {
    ipc_send=recv();
    uint64 descriptor=ipc_send.a0;
    uint64 buf=ipc_send.a1;
    uint64 size=ipc_send.a2;
    uint64 namesrc=ipc_send.a3;
    uint64 param=ipc_send.a4;
    uint64 syscallnum=ipc_send.a5;
    // проверка номера вызова
    switch (syscallnum){
      //write
      case 1: 
        //проврека дескриптора
        switch (descriptor) {
          case 1:
            // перенаправление в консоль
            // проврека параметра буфера a4
            // если 1 то коирование из IPC буфера в IPC буфрер терминала
            // если 2 то коирование через copyin в IPC буфер терминала
            if (param==1){
            ipc_buf_cpy(terminal,namesrc,size);// обращение не по имени ошибка !!!
            send(0,0,0,0,0,0,terminal);
            } else {
              buf2ipc_cpy(terminal,namesrc,buf,size);
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
        // send(namesrc,buf,size,0,0,0,uinput);
        break;
    }
  }
}

