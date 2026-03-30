#pragma once

typedef int bool;
typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef uint32 size_t;
typedef uint32 paddr_t;
typedef uint32 vaddr_t;

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg
#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)


int printf(char *fmt,...);
void putchar(char ch);
void puts(const char *str); 
void *memset(void *dst,int c,uint n);
void* memmove(void *vdst, const void *vsrc, int n);
void panic(char *s);
