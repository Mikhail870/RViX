// Тут настраиваются прерывания
// Пока что разоешить прерывания глобально, для таймера и бросить kernelvec в stvec
// Добавить UART
// Иные  устройтсва
#include "types.h"
#include "PLIC.h"

void init_interrupt_enable(void){
  asm volatile("csrs sstatus, %0" : : "r"(1 << 1));// Разрешить прерывания глобально
  asm volatile("csrs sie, %0" : : "r"(1 << 5));// Сохранять значение SIE до ловушки прерывания
  asm volatile("csrs sie, %0" : : "r"(1 << 7));// Разрешить прерывания таймера

}
void set_stvec(void *stvec){
// Устанавливаем регистр stvec
 asm volatile("csrw stvec, %0" : : "r"(stvec));
}
