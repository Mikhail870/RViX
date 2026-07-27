int send(uint ID_dest,long arg0,long arg1,long arg2,long arg3,long arg7){
  register long a0 __asm__("a0")=arg0;
  register long a1 __asm__("a1")=arg1;
  register long a2 __asm__("a2")=arg2;
  register long a3 __asm__("a3")=arg3;
  register long a7 __asm__("a7")=arg7;

  __asm__ __volatile__("ecall"
                         : "=r"(a0)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3),"r"(a7)
                         : "memory");

  return a1;
}

