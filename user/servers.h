uint64 ipc_buf_cpy(uint64 dst, uint64 src, int size);
uint64 buf2ipc_cpy(uint64 dst,uint64 src,uint64 bfadr_src,int size);
uint64 sleep(uint64);
uint64 wakeup(uint64);


// работа с вводом выводом
#define UART 0x3fffffc000
#define Reg(reg) ((volatile unsigned char *)(UART + (reg)))
#define ReadReg(reg)     (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))
#define LSR 5
#define RHR 0
#define LSR_RX_READY (1 << 0)
