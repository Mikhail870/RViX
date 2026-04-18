#include "common.h"
 __attribute__ ((aligned (4)))  void test(void){
  printf("interrupt !");
  printf("OpenSBI надо тоже мапить !");
  PANIC("stop !");
}

