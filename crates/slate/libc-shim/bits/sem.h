#ifndef _SLATE_BITS_SEM_H
#define _SLATE_BITS_SEM_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/sem.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/sem.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/sem.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/sem.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/sem.h>
#else
struct semid_ds {
  struct ipc_perm sem_perm;
  time_t          sem_otime;
  time_t          sem_ctime;
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned short sem_nsems;
  char __sem_nsems_pad[sizeof(long) - sizeof(short)];
#else
  char __sem_nsems_pad[sizeof(long) - sizeof(short)];
  unsigned short sem_nsems;
#endif
  long __unused3;
  long __unused4;
};
#endif

#endif /* _SLATE_BITS_SEM_H */
