#include "lib.h"
void main(void){
putchar('1');
//putchar('\n');
puts("from IPC buffer !!");
send(54321,8000,9000,9500,9700,9900,2);
while(1){
    putchar('1');
    putchar('\n');
  }
}
