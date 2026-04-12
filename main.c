#include "types.h"
#include "riscv.h"
#include "memlayout.h"
#include "common.h"
#include "vm.h"
#include "defs.h"
#include "kalloc.h"
extern pagetable_t kernel_pagetable;

void  main() {
  printf("kernel is booting ! \n");
// 1. Инициализируем физический аллокатор (kalloc.c)
kinit(); 

// 2. Создаем таблицу страниц ядра (vm.c)
// Внутри этой функции обычно вызывается kvmmap для девайсов и кода
kvminit(); 

// 3. Наш тест: берем одну физическую страницу
void *phys_mem = kalloc();
if(phys_mem == 0) panic("kalloc failed");

uint64 va = 0x40000000; // Виртуальный адрес

// 4. Мапим: Виртуальный -> Физический
// kernel_pagetable — это глобальная переменная из vm.c
kvmmap(kernel_pagetable, va, (uint64)phys_mem, PGSIZE, PTE_R | PTE_W);

printf("Test mapping: VA 0x%p -> PA 0x%p\n", va, phys_mem);

// 5. ВКЛЮЧАЕМ ПЕЙДЖИНГ (Самый ответственный момент)
kvminithart(); 
printf("Paging enabled!\n");

// 6. Проверка: пишем по виртуальному адресу
char *ptr = (char*)va;
*ptr = 'A'; 

if(*((char*)phys_mem) == 'A') {
    printf("It works! Physical memory at 0x%p contains: %c\n", phys_mem, *ptr);
}
for(;;);
 __asm__ __volatile__("wfi");
}
