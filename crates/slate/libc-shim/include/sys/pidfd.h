#ifndef _SLATE_SYS_PIDFD_H
#define _SLATE_SYS_PIDFD_H

#include <stdint.h>

struct pidfd_info {
  uint64_t mask;
  uint64_t cgroupid;
  uint32_t pid;
  uint32_t tgid;
  uint32_t ppid;
  uint32_t ruid;
  uint32_t rgid;
  uint32_t euid;
  uint32_t egid;
  uint32_t suid;
  uint32_t sgid;
  uint32_t fsuid;
  uint32_t fsgid;
  int32_t exit_code;
  uint32_t coredump_mask;
#if !defined(__SLATE_ARCH_AARCH64)
  uint32_t coredump_signal;
  uint32_t coredump_code;
  uint32_t coredump_pad;
  uint64_t supported_mask;
#else
  uint32_t __spare1;
#endif
};

#endif
