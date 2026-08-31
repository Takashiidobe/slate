#if !defined(_SLATE_LIBC)
#error "Never include <bits/x86/sem.h> directly; include a public header instead."
#endif

struct semid_ds {
  struct ipc_perm sem_perm;
  unsigned long   __sem_otime_lo;
  unsigned long   __sem_otime_hi;
  unsigned long   __sem_ctime_lo;
  unsigned long   __sem_ctime_hi;
  unsigned short  sem_nsems;
  char            __sem_nsems_pad[sizeof(long) - sizeof(short)];
  long            __unused3;
  long            __unused4;
  time_t          sem_otime;
  time_t          sem_ctime;
};
