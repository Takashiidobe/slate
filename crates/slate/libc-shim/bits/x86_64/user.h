typedef struct user_fpregs_struct {
  unsigned short int cwd;
  unsigned short int swd;
  unsigned short int ftw;
  unsigned short int fop;
  unsigned long int  rip;
  unsigned long int  rdp;
  unsigned int       mxcsr;
  unsigned int       mxcr_mask;
  unsigned int       st_space[32];
  unsigned int       xmm_space[64];
  unsigned int       padding[24];
} elf_fpregset_t;

struct user_regs_struct {
  unsigned long r15;
  unsigned long r14;
  unsigned long r13;
  unsigned long r12;
  unsigned long rbp;
  unsigned long rbx;
  unsigned long r11;
  unsigned long r10;
  unsigned long r9;
  unsigned long r8;
  unsigned long rax;
  unsigned long rcx;
  unsigned long rdx;
  unsigned long rsi;
  unsigned long rdi;
  unsigned long orig_rax;
  unsigned long rip;
  unsigned long cs;
  unsigned long eflags;
  unsigned long rsp;
  unsigned long ss;
  unsigned long fs_base;
  unsigned long gs_base;
  unsigned long ds;
  unsigned long es;
  unsigned long fs;
  unsigned long gs;
};

#define ELF_NGREG 27
typedef unsigned long long elf_greg_t, elf_gregset_t[ELF_NGREG];

struct user {
  struct user_regs_struct    regs;
  int                        u_fpvalid;
  struct user_fpregs_struct  i387;
  unsigned long              u_tsize;
  unsigned long              u_dsize;
  unsigned long              u_ssize;
  unsigned long              start_code;
  unsigned long              start_stack;
  long                       signal;
  int                        reserved;
  struct user_regs_struct   *u_ar0;
  struct user_fpregs_struct *u_fpstate;
  unsigned long              magic;
  char                       u_comm[32];
  unsigned long              u_debugreg[8];
};
