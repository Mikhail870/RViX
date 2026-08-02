#include "lib.h"
void main(void){
struct msg mess;
mess=recv();
printk(mess.a2);
}
