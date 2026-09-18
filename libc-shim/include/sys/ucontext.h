#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)
#include <bits/darwin/ucontext.h>
#elif defined(__SLATE_LIBC_GLIBC)
#if defined(__SLATE_ARCH_AARCH64)
#include <sys/types.h>
#endif
#define __NEED_size_t
#define __NEED_sigset_t
#include <bits/types.h>
struct sigaltstack;
typedef struct sigaltstack stack_t;
#include <bits/signal.h>
#if defined(__SLATE_ARCH_X86_64)
#define NGREG 23
#elif defined(__SLATE_ARCH_X86)
#define NGREG 19
#elif defined(__SLATE_ARCH_AARCH64)
#define NGREG 34
#elif defined(__SLATE_ARCH_ARM)
#define NGREG 18
#define REG_R0 0
#define REG_R1 1
#define REG_R2 2
#define REG_R3 3
#define REG_R4 4
#define REG_R5 5
#define REG_R6 6
#define REG_R7 7
#define REG_R8 8
#define REG_R9 9
#define REG_R10 10
#define REG_R11 11
#define REG_R12 12
#define REG_R13 13
#define REG_R14 14
#define REG_R15 15
#endif
#else
#include <ucontext.h>
#endif
