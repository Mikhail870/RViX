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

struct IPC_reg IPC_call(void);
struct IPC_reg send(void);
void recv(void);
struct IPC_reg retregisters(struct process *prc);
void copy_reg(struct process *src,struct process *dst);
struct process *find_name_process(uint64 name);
void sleep(struct process *prc);
void runable(struct process* prc);
void add_que(struct process *prc,uint64 name);
uint64 extract_que(struct process *prc);
uint64 get_dst_name(void);
