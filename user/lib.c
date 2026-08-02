// функция делает ipc вызов, отправляющий
// a0-a5 полезное сообщение
// a6 - имя получаетеля
// в регистр a7 автоматически подставляется код вызова
// функция возвращает код в a0;
int send(long arg0,long arg1,long arg2,
         long arg3, long arg4, long arg5,long arg6,long arg7){

  register long a0 __asm__("a0")=arg0;
  register long a1 __asm__("a1")=arg1;
  register long a2 __asm__("a2")=arg2;
  register long a3 __asm__("a3")=arg3;
  register long a4 __asm__("a4")=arg4;
  register long a5 __asm__("a5")=arg5;
  register long a6 __asm__("a6")=arg6;
  register long a7 __asm__("a7")=0;

  __asm__ __volatile__("ecall"
                         : "+r"(a0)
                         : "r"(a1), "r"(a2), "r"(a3), "r"(a4),
     "r"(a5), "r"(a6),"r"(a7)
                         : "memory");

  return a0;
}
