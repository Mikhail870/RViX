#include "lib.h"
void main(void){
printk(222222);
struct msg mess;
while(1){
mess=recv();
printk(mess.a0);
}
}
