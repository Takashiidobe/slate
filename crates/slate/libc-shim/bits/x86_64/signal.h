#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
enum {
  MINSIGSTKSZ = 2048,
  SIGSTKSZ    = 8192,
};
#endif

#ifdef _GNU_SOURCE
enum {
  REG_R8      = 0,
  REG_R9      = 1,
  REG_R10     = 2,
  REG_R11     = 3,
  REG_R12     = 4,
  REG_R13     = 5,
  REG_R14     = 6,
  REG_R15     = 7,
  REG_RDI     = 8,
  REG_RSI     = 9,
  REG_RBP     = 10,
  REG_RBX     = 11,
  REG_RDX     = 12,
  REG_RAX     = 13,
  REG_RCX     = 14,
  REG_RSP     = 15,
  REG_RIP     = 16,
  REG_EFL     = 17,
  REG_CSGSFS  = 18,
  REG_ERR     = 19,
  REG_TRAPNO  = 20,
  REG_OLDMASK = 21,
  REG_CR2     = 22,
};

#define REG_R8      REG_R8
#define REG_R9      REG_R9
#define REG_R10     REG_R10
#define REG_R11     REG_R11
#define REG_R12     REG_R12
#define REG_R13     REG_R13
#define REG_R14     REG_R14
#define REG_R15     REG_R15
#define REG_RDI     REG_RDI
#define REG_RSI     REG_RSI
#define REG_RBP     REG_RBP
#define REG_RBX     REG_RBX
#define REG_EFL     REG_EFL
#define REG_RDX     REG_RDX
#define REG_RAX     REG_RAX
#define REG_RCX     REG_RCX
#define REG_RSP     REG_RSP
#define REG_RIP     REG_RIP
#define REG_CSGSFS  REG_CSGSFS
#define REG_ERR     REG_ERR
#define REG_TRAPNO  REG_TRAPNO
#define REG_OLDMASK REG_OLDMASK
#define REG_CR2     REG_CR2
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef long long greg_t, gregset_t[23];
typedef struct _fpstate {
  unsigned short     cwd, swd, ftw, fop;
  unsigned long long rip, rdp;
  unsigned           mxcsr, mxcr_mask;
  struct {
    unsigned short significand[4], exponent, padding[3];
  } _st[8];
  struct {
    unsigned element[4];
  } _xmm[16];
  unsigned padding[24];
} *fpregset_t;
struct sigcontext {
  unsigned long    r8, r9, r10, r11, r12, r13, r14, r15;
  unsigned long    rdi, rsi, rbp, rbx, rdx, rax, rcx, rsp, rip, eflags;
  unsigned short   cs, gs, fs, __pad0;
  unsigned long    err, trapno, oldmask, cr2;
  struct _fpstate *fpstate;
  unsigned long    __reserved1[8];
};
typedef struct {
  gregset_t          gregs;
  fpregset_t         fpregs;
  unsigned long long __reserved1[8];
} mcontext_t;
#else
typedef struct {
  unsigned long __space[32];
} mcontext_t;
#endif

struct sigaltstack {
  void  *ss_sp;
  int    ss_flags;
  size_t ss_size;
};

typedef struct __ucontext {
  unsigned long      uc_flags;
  struct __ucontext *uc_link;
  stack_t            uc_stack;
  mcontext_t         uc_mcontext;
  sigset_t           uc_sigmask;
  unsigned long      __fpregs_mem[64];
} ucontext_t;

#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO   4
#define SA_ONSTACK   0x08000000
#define SA_RESTART   0x10000000
#define SA_NODEFER   0x40000000
#define SA_RESETHAND 0x80000000
#define SA_RESTORER  0x04000000

#endif

enum {
  SIGHUP    = 1,
  SIGINT    = 2,
  SIGQUIT   = 3,
  SIGILL    = 4,
  SIGTRAP   = 5,
  SIGABRT   = 6,
  SIGIOT    = SIGABRT,
  SIGBUS    = 7,
  SIGFPE    = 8,
  SIGKILL   = 9,
  SIGUSR1   = 10,
  SIGSEGV   = 11,
  SIGUSR2   = 12,
  SIGPIPE   = 13,
  SIGALRM   = 14,
  SIGTERM   = 15,
  SIGSTKFLT = 16,
  SIGCHLD   = 17,
  SIGCONT   = 18,
  SIGSTOP   = 19,
  SIGTSTP   = 20,
  SIGTTIN   = 21,
  SIGTTOU   = 22,
  SIGURG    = 23,
  SIGXCPU   = 24,
  SIGXFSZ   = 25,
  SIGVTALRM = 26,
  SIGPROF   = 27,
  SIGWINCH  = 28,
  SIGIO     = 29,
  SIGPOLL   = 29,
  SIGPWR    = 30,
  SIGSYS    = 31,
  SIGUNUSED = SIGSYS,
  _NSIG     = 65,
};
