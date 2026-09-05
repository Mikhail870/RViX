// сервер файловой системы
// принимает запросы от программ
// управляет дескрипторами
#include "lib.h"

struct msg ipc_send;
void main(void){
  while (1) {
  ipc_send=recv();
  switch (ipc_send.a0) {
    case 1:
  // перенаправление в консоль
  break;
    case 2:
  // поток ошибок
  break;
  case 3:
  // работа с файлом (перенаправление к фс)
  break;
  }
  }
}
