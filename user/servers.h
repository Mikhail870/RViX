uint64 ipc_buf_cpy(uint64 dst, uint64 src, int size);
uint64 buf2ipc_cpy(uint64 dst,uint64 src,uint64 bfadr_src,int size);
uint64 sleep(uint64);
uint64 wakeup(uint64);
#define UART 0x3fffffc000
