#define MAX_PROS 8
#define SLEEP 3
#define RUN 2
#define RUNABBLE 1
#define UNUSED 0
#define PAGE 4096 


// Тут сохраняется контекст и указтель стека с адресом возврата
// До этого контекст сохранялся на стеке ядра, теперь для этого есть структура
// Структура context взята из xv6

struct context {
  uint64 ra;
  uint64 sp;

  // callee-saved
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

// тут добавлен модуль для работы ipc
struct ipc {
  uint64 name; // имя процесса, назанчается ОС
  uint64 que[MAX_PROS]; // очеред в ipc из отправителей
};

// тут добавден trapframe, струткура для сохранения регистров прри переходи U mode <-> S mode
// Структура trapframe взята из xv6
struct trapframe {
  /*   0 */ uint64 kernel_satp;   // kernel page table
  /*   8 */ uint64 kernel_sp;     // top of process's kernel stack
  /*  16 */ uint64 kernel_trap;   // usertrap()
  /*  24 */ uint64 epc;           // saved user program counter
  /*  32 */ uint64 kernel_hartid; // saved kernel tp
  /*  40 */ uint64 ra;
  /*  48 */ uint64 sp;
  /*  56 */ uint64 gp;
  /*  64 */ uint64 tp;
  /*  72 */ uint64 t0;
  /*  80 */ uint64 t1;
  /*  88 */ uint64 t2;
  /*  96 */ uint64 s0;
  /* 104 */ uint64 s1;
  /* 112 */ uint64 a0;
  /* 120 */ uint64 a1;
  /* 128 */ uint64 a2;
  /* 136 */ uint64 a3;
  /* 144 */ uint64 a4;
  /* 152 */ uint64 a5;
  /* 160 */ uint64 a6;
  /* 168 */ uint64 a7;
  /* 176 */ uint64 s2;
  /* 184 */ uint64 s3;
  /* 192 */ uint64 s4;
  /* 200 */ uint64 s5;
  /* 208 */ uint64 s6;
  /* 216 */ uint64 s7;
  /* 224 */ uint64 s8;
  /* 232 */ uint64 s9;
  /* 240 */ uint64 s10;
  /* 248 */ uint64 s11;
  /* 256 */ uint64 t3;
  /* 264 */ uint64 t4;
  /* 272 */ uint64 t5;
  /* 280 */ uint64 t6;
};


// структура процесса
 struct process {
  int pid; // номер процессами
  int state; // состояние процесса
  uint64 *sp; // указатель стека процесса
  //uint64 kstack[PAGE/8]; // ядерный стек процесса
  uint64 stack[PAGE/8] __attribute__((aligned(16))); // пользовательский стек процесса

  struct ipc *ipc_data; // блок ipc 
  struct trapframe *trapframe;
  struct context context;
  pagetable_t pagetable; // таблица странц
  uint64 kstack;
};

extern struct process proc[MAX_PROS]; // структуры под процессы глобально для всех
extern struct process *next;
extern struct process * volatile current;
void prepare_uret(void);
void proc_born(uint64 *prc,uint64 size);
struct process *init_process(int param);
void sheduller(void);
void switch_context(struct context*,struct context* );
void yield(void);
