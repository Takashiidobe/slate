#include <sched.h>

_Static_assert(__builtin_offsetof(struct sched_param, sched_priority) == 0, "struct sched_param.sched_priority offset differs from oracle");

typedef int slate_oracle_struct_sched_param_sched_priority;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sched_param *)0)->sched_priority), slate_oracle_struct_sched_param_sched_priority), "struct sched_param.sched_priority field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sched_param, __reserved1) == 4, "struct sched_param.__reserved1 offset differs from oracle");

typedef int slate_oracle_struct_sched_param___reserved1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sched_param *)0)->__reserved1), slate_oracle_struct_sched_param___reserved1), "struct sched_param.__reserved1 field type differs from oracle");

typedef int slate_oracle_struct_sched_param___reserved3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sched_param *)0)->__reserved3), slate_oracle_struct_sched_param___reserved3), "struct sched_param.__reserved3 field type differs from oracle");

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

#ifndef SCHED_FIFO
#error "sched.h:SCHED_FIFO macro is missing from libc-shim"
#endif

#ifndef SCHED_IDLE
#error "sched.h:SCHED_IDLE macro is missing from libc-shim"
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

int main(void) { return 0; }
