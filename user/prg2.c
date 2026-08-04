#include "lib.h"
void main(void){
printk(2);
struct msg mess;
mess=recv();
printk(mess.a2);
}
