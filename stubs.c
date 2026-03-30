#include "types.h"
#include "riscv.h"
#include "defs.h"

// Блокировки и прерывания
void initlock(struct spinlock *lk, char *name) { (void)lk; (void)name; }
void acquire(struct spinlock *lk) { (void)lk; }
void release(struct spinlock *lk) { (void)lk; }
int  holding(struct spinlock *lk) { (void)lk; return 1; }
void push_off(void) {}
void pop_off(void) {}

// Строки (memcpy уже есть в common.c, тут не нужна)
char* safestrcpy(char *s, const char *t, int n) { (void)t; (void)n; return s; }

// Файловая система (типы должны совпадать с defs.h)
struct inode* namei(char *path) { (void)path; return 0; }
struct file*  filedup(struct file *f) { return f; }
struct inode* idup(struct inode *i) { return i; }
void          fileclose(struct file *f) { (void)f; }
void          iput(struct inode *i) { (void)i; }
void          begin_op() {}
void          end_op() {}
void          fsinit(int dev) { (void)dev; }

// Процессы и ядро
void swtch(struct context *old, struct context *new) { (void)old; (void)new; }
int  kexec(char *path, char **argv) { (void)path; (void)argv; return -1; }
void prepare_return() {}
