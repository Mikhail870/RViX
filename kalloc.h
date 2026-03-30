void *kalloc(void);
void  kfree(void *);
void  kinit(void);

#define PGROUNDUP(sz) (((sz) + 4096 - 1) & ~(4096 - 1))
#define PGSIZE 4096
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)
