#include "lib.h"
void main(void){
putchar('2');
putchar('\n');
struct msg mess;
while(1){
mess=recv();
printk(mess.a0);
}
}
