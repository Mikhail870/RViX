#include "lib.h"

int main(){
char* strmem=(char*)IPC_BUFF;
puts(" \n");
puts(" \n");
puts("RViX shell v.0\n");
puts(strmem);
  while(1) {
puts(">");
strmem=gets(strmem);
if(strcmp("\r",strmem)==0){
      continue;
    } else if(strcmp("uname\r",strmem)==0){
      puts("RViX microkernel V.0\n");
    } else if(strcmp("test\r",strmem)==0){
      int pid=fork();
      if(pid!=0)
        puts("its parent\n");
    }
    else {
      puts("unknow command\n");
    }
  }
}

