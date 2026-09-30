#include <sched.h>

extern int slate_oracle_clone(int (*)(void *), void *, int, void *, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clone), __typeof__(clone)),
    "sched.h:clone declaration differs from oracle");

static __typeof__(clone) *const slate_reference_clone = &clone;

extern int slate_oracle_sched_setparam(int, const struct sched_param *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sched_setparam), __typeof__(sched_setparam)),
    "sched.h:sched_setparam declaration differs from oracle");

static __typeof__(sched_setparam) *const slate_reference_sched_setparam = &sched_setparam;

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

#ifndef CLONE_CHILD_CLEARTID
#error "sched.h:CLONE_CHILD_CLEARTID macro is missing from libc-shim"
#endif

#ifndef CLONE_CHILD_SETTID
#error "sched.h:CLONE_CHILD_SETTID macro is missing from libc-shim"
#endif

#ifndef CLONE_DETACHED
#error "sched.h:CLONE_DETACHED macro is missing from libc-shim"
#endif

#ifndef CLONE_FILES
#error "sched.h:CLONE_FILES macro is missing from libc-shim"
#endif

#ifndef CLONE_FS
#error "sched.h:CLONE_FS macro is missing from libc-shim"
#endif

#ifndef CLONE_IO
#error "sched.h:CLONE_IO macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWCGROUP
#error "sched.h:CLONE_NEWCGROUP macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWIPC
#error "sched.h:CLONE_NEWIPC macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWNET
#error "sched.h:CLONE_NEWNET macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWNS
#error "sched.h:CLONE_NEWNS macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWPID
#error "sched.h:CLONE_NEWPID macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWTIME
#error "sched.h:CLONE_NEWTIME macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWUSER
#error "sched.h:CLONE_NEWUSER macro is missing from libc-shim"
#endif

#ifndef CLONE_NEWUTS
#error "sched.h:CLONE_NEWUTS macro is missing from libc-shim"
#endif

#ifndef CLONE_PARENT
#error "sched.h:CLONE_PARENT macro is missing from libc-shim"
#endif

#ifndef CLONE_PARENT_SETTID
#error "sched.h:CLONE_PARENT_SETTID macro is missing from libc-shim"
#endif

#ifndef CLONE_PIDFD
#error "sched.h:CLONE_PIDFD macro is missing from libc-shim"
#endif

#ifndef CLONE_PTRACE
#error "sched.h:CLONE_PTRACE macro is missing from libc-shim"
#endif

#ifndef CLONE_SETTLS
#error "sched.h:CLONE_SETTLS macro is missing from libc-shim"
#endif

#ifndef CLONE_SIGHAND
#error "sched.h:CLONE_SIGHAND macro is missing from libc-shim"
#endif

#ifndef CLONE_SYSVSEM
#error "sched.h:CLONE_SYSVSEM macro is missing from libc-shim"
#endif

#ifndef CLONE_THREAD
#error "sched.h:CLONE_THREAD macro is missing from libc-shim"
#endif

#ifndef CLONE_UNTRACED
#error "sched.h:CLONE_UNTRACED macro is missing from libc-shim"
#endif

#ifndef CLONE_VFORK
#error "sched.h:CLONE_VFORK macro is missing from libc-shim"
#endif

#ifndef CLONE_VM
#error "sched.h:CLONE_VM macro is missing from libc-shim"
#endif

#ifndef CPU_ALLOC
#error "sched.h:CPU_ALLOC macro is missing from libc-shim"
#endif

#ifndef CPU_ALLOC_SIZE
#error "sched.h:CPU_ALLOC_SIZE macro is missing from libc-shim"
#endif

#ifndef CPU_AND
#error "sched.h:CPU_AND macro is missing from libc-shim"
#endif

#ifndef CPU_AND_S
#error "sched.h:CPU_AND_S macro is missing from libc-shim"
#endif

#ifndef CPU_CLR
#error "sched.h:CPU_CLR macro is missing from libc-shim"
#endif

#ifndef CPU_CLR_S
#error "sched.h:CPU_CLR_S macro is missing from libc-shim"
#endif

#ifndef CPU_COUNT
#error "sched.h:CPU_COUNT macro is missing from libc-shim"
#endif

#ifndef CPU_COUNT_S
#error "sched.h:CPU_COUNT_S macro is missing from libc-shim"
#endif

#ifndef CPU_EQUAL
#error "sched.h:CPU_EQUAL macro is missing from libc-shim"
#endif

#ifndef CPU_EQUAL_S
#error "sched.h:CPU_EQUAL_S macro is missing from libc-shim"
#endif

#ifndef CPU_FREE
#error "sched.h:CPU_FREE macro is missing from libc-shim"
#endif

#ifndef CPU_ISSET
#error "sched.h:CPU_ISSET macro is missing from libc-shim"
#endif

#ifndef CPU_ISSET_S
#error "sched.h:CPU_ISSET_S macro is missing from libc-shim"
#endif

#ifndef CPU_OR
#error "sched.h:CPU_OR macro is missing from libc-shim"
#endif

#ifndef CPU_OR_S
#error "sched.h:CPU_OR_S macro is missing from libc-shim"
#endif

#ifndef CPU_SET
#error "sched.h:CPU_SET macro is missing from libc-shim"
#endif

#ifndef CPU_SETSIZE
#error "sched.h:CPU_SETSIZE macro is missing from libc-shim"
#endif

#ifndef CPU_SET_S
#error "sched.h:CPU_SET_S macro is missing from libc-shim"
#endif

#ifndef CPU_XOR
#error "sched.h:CPU_XOR macro is missing from libc-shim"
#endif

#ifndef CPU_XOR_S
#error "sched.h:CPU_XOR_S macro is missing from libc-shim"
#endif

#ifndef CPU_ZERO
#error "sched.h:CPU_ZERO macro is missing from libc-shim"
#endif

#ifndef CPU_ZERO_S
#error "sched.h:CPU_ZERO_S macro is missing from libc-shim"
#endif

#ifndef CSIGNAL
#error "sched.h:CSIGNAL macro is missing from libc-shim"
#endif

#ifndef SCHED_BATCH
#error "sched.h:SCHED_BATCH macro is missing from libc-shim"
#endif

#ifndef SCHED_DEADLINE
#error "sched.h:SCHED_DEADLINE macro is missing from libc-shim"
#endif

#ifndef SCHED_EXT
#error "sched.h:SCHED_EXT macro is missing from libc-shim"
#endif

#ifndef SCHED_FIFO
#error "sched.h:SCHED_FIFO macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_DL_OVERRUN
#error "sched.h:SCHED_FLAG_DL_OVERRUN macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_KEEP_ALL
#error "sched.h:SCHED_FLAG_KEEP_ALL macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_KEEP_PARAMS
#error "sched.h:SCHED_FLAG_KEEP_PARAMS macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_KEEP_POLICY
#error "sched.h:SCHED_FLAG_KEEP_POLICY macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_RECLAIM
#error "sched.h:SCHED_FLAG_RECLAIM macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_RESET_ON_FORK
#error "sched.h:SCHED_FLAG_RESET_ON_FORK macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_UTIL_CLAMP
#error "sched.h:SCHED_FLAG_UTIL_CLAMP macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_UTIL_CLAMP_MAX
#error "sched.h:SCHED_FLAG_UTIL_CLAMP_MAX macro is missing from libc-shim"
#endif

#ifndef SCHED_FLAG_UTIL_CLAMP_MIN
#error "sched.h:SCHED_FLAG_UTIL_CLAMP_MIN macro is missing from libc-shim"
#endif

#ifndef SCHED_IDLE
#error "sched.h:SCHED_IDLE macro is missing from libc-shim"
#endif

#ifndef SCHED_ISO
#error "sched.h:SCHED_ISO macro is missing from libc-shim"
#endif

#ifndef SCHED_NORMAL
#error "sched.h:SCHED_NORMAL macro is missing from libc-shim"
#endif

#ifndef SCHED_OTHER
#error "sched.h:SCHED_OTHER macro is missing from libc-shim"
#endif

#ifndef SCHED_RESET_ON_FORK
#error "sched.h:SCHED_RESET_ON_FORK macro is missing from libc-shim"
#endif

#ifndef SCHED_RR
#error "sched.h:SCHED_RR macro is missing from libc-shim"
#endif

#ifndef sched_priority
#error "sched.h:sched_priority macro is missing from libc-shim"
#endif

int main(void) { return 0; }
