#define MAX_PROS 8
#define RUN 2
#define RUNABBLE 1
#define UNUSED 0
#define PAGE 4096 

// структура процесса
 struct process {
  int pid; // номер процессами
  int state; // состояние процесса
  uint64 *sp; // указатель стека процесса
  uint64 kstack[PAGE/8]; // ядерный стек процесса
  uint64 stack[PAGE/8] __attribute__((aligned(16))); // пользовательский стек процесса
};

extern struct process proc[MAX_PROS]; // структуры под процессы глобально для всех
extern struct process *next;
extern struct process *current;
struct process *proc_born(uint64 prc);
struct process *init_process(int param);
void sheduller(void);
void switch_context(uint64 **oldsp, uint64 **newsp);
void yield(void);
