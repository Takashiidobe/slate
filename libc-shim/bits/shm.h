#ifndef _SLATE_BITS_SHM_H
#define _SLATE_BITS_SHM_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/shm.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/shm.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/shm.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/shm.h>
#else
#define SHMLBA 4096

struct shmid_ds {
  struct ipc_perm shm_perm;
  size_t          shm_segsz;
  time_t          shm_atime;
  time_t          shm_dtime;
  time_t          shm_ctime;
  pid_t           shm_cpid;
  pid_t           shm_lpid;
  unsigned long   shm_nattch;
  unsigned long   __pad1;
  unsigned long   __pad2;
};

struct shminfo {
  unsigned long shmmax, shmmin, shmmni, shmseg, shmall, __unused[4];
};

struct shm_info {
  int           __used_ids;
  unsigned long shm_tot, shm_rss, shm_swp;
  unsigned long __swap_attempts, __swap_successes;
};
#endif

#endif /* _SLATE_BITS_SHM_H */
