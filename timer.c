#include "types.h"
#include "HAL.h"
#include "timer.h"

/*
 Системный таймер.
 Реализует:
 Установка таймера set_timer()
 Получение системного времени get_time()
 Установка таймера с заданным повтором auto_timer()
*/

 uint64 get_time(void) {
    uint64 t;
    asm volatile ("rdtime %0" : "=r" (t)); //чтение системного регистра лучше перенести в HAL.c
    return t;
}

void set_timer(uint64 delta_time){
  uint64 next_time=get_time()+delta_time;
  sbi_set_timer(next_time);
}

void auto_timer(){
  
}
