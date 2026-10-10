#define SBRK_EAGER 1
#define SBRK_LAZY  2
void kvminit(void);
void *kalloc(void);
pagetable_t uvmcreate(void);
void uvmunmap(pagetable_t, uint64, uint64, int);
int mappages(pagetable_t, uint64, uint64, uint64, int);
void uvmfree(pagetable_t, uint64);
uint64 walkaddr(pagetable_t,uint64 va0);
int uvmcopy(pagetable_t old, pagetable_t new, uint64 sz);
