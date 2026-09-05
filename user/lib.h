#include "types.h"
struct msg {
    long a0;
    long a1;
    long a2;
    long a3;
    long a4;
    long a5;
    long a6;
    long a7;
};

int send(long arg0,long arg1,long arg2,
         long arg3, long arg4, long arg5,long arg6);


struct msg recv(void);
void printk(long x);
ssize_t write(int fd, const void *buf, size_t count);
