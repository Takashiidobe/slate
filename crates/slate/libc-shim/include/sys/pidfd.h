#ifndef _SLATE_SYS_PIDFD_H
#define _SLATE_SYS_PIDFD_H

#if defined(__SLATE_LIBC_GLIBC)
#include <fcntl.h>
#include <sys/ioctl.h>
#endif
#include <stdint.h>

#if defined(__SLATE_LIBC_GLIBC) && !defined(__SLATE_ARCH_X86) && !defined(__SLATE_ARCH_X86_64)
int pidfd_open(int, unsigned int);
#endif

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

#if defined(__SLATE_LIBC_GLIBC)
#define PIDFD_NONBLOCK O_NONBLOCK
#define PIDFD_THREAD O_EXCL
#define PIDFD_SIGNAL_THREAD (1UL << 0)
#define PIDFD_SIGNAL_THREAD_GROUP (1UL << 1)
#define PIDFD_SIGNAL_PROCESS_GROUP (1UL << 2)
#define PIDFS_IOCTL_MAGIC 0xFF
#define PIDFD_SELF_THREAD -10000
#define PIDFD_SELF_THREAD_GROUP -10001
#define PIDFD_SELF PIDFD_SELF_THREAD
#define PIDFD_SELF_PROCESS PIDFD_SELF_THREAD_GROUP
#define PIDFD_INFO_PID (1UL << 0)
#define PIDFD_INFO_CREDS (1UL << 1)
#define PIDFD_INFO_CGROUPID (1UL << 2)
#define PIDFD_INFO_EXIT (1UL << 3)
#define PIDFD_INFO_COREDUMP (1UL << 4)
#define PIDFD_INFO_SUPPORTED_MASK (1UL << 5)
#define PIDFD_INFO_COREDUMP_SIGNAL (1UL << 6)
#define PIDFD_INFO_COREDUMP_CODE (1UL << 7)
#define PIDFD_COREDUMPED (1U << 0)
#define PIDFD_COREDUMP_SKIP (1U << 1)
#define PIDFD_COREDUMP_USER (1U << 2)
#define PIDFD_COREDUMP_ROOT (1U << 3)
#define PIDFD_INFO_SIZE_VER0 64
#define PIDFD_INFO_SIZE_VER1 72
#define PIDFD_INFO_SIZE_VER2 80
#define PIDFD_INFO_SIZE_VER3 88
#define PIDFD_GET_CGROUP_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 1)
#define PIDFD_GET_IPC_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 2)
#define PIDFD_GET_MNT_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 3)
#define PIDFD_GET_NET_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 4)
#define PIDFD_GET_PID_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 5)
#define PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 6)
#define PIDFD_GET_TIME_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 7)
#define PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 8)
#define PIDFD_GET_USER_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 9)
#define PIDFD_GET_UTS_NAMESPACE _IO(PIDFS_IOCTL_MAGIC, 10)
#define PIDFD_GET_INFO _IOWR(PIDFS_IOCTL_MAGIC, 11, struct pidfd_info)
#endif

#endif
