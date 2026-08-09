#include "lib.h"
void main(void){
printk(222222);
struct msg mess;
for(int i=0;i<2;i++){
mess=recv();
printk(mess.a0);
}
}
