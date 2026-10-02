#include "lib.h"

int main(){
char* strmem=(char*)IPC_BUFF;
puts("test shell \n");
puts("coming soon keyboard input are enabled !\n");
puts(strmem);
  while(1) {
puts(">");
strmem=gets(strmem);
  }
}
