#include "common.h"
#include "kalloc.h"
#include "vm.h"
void  main() {
  printf("kernel is booting ! \n");
  kinit();
  kvminit();



for(;;);
 __asm__ __volatile__("wfi");
}
