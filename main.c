#include "common.h"
#include "kalloc.h"
#include "vm.h"
void  main() {
  kinit();
  kvminit();



for(;;);
 __asm__ __volatile__("wfi");
}
