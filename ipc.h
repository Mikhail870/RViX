struct IPC_reg {
  uint64 a0;
  uint64 a1;
  uint64 a2;
  uint64 a3;
  uint64 a4;
  uint64 a5;
  uint64 a6;
  uint64 a7;
};

void IPC_call(void);
void copy_reg(struct process *src,struct process *dst);
struct process *find_name_process(uint64 name);
void sleep(struct process *prc);
void add_que(struct process *prc,uint64 name);
