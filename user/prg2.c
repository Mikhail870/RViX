#include "lib.h"
void main(void){
printk(222222);
struct msg mess;
for(int i=0;i<2;i++){
mess=recv();
printk(mess.a0);
printk(mess.a1);
printk(mess.a2);
printk(mess.a3);
printk(mess.a4);
printk(mess.a5);
printk(mess.a6);
}
}
