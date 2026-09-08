#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#define _GNU_SOURCE
#endif

#include <arpa/inet.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/user.h>
#include <termios.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

#if defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_X86_64)
#include <sys/reg.h>
#endif

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#include <netinet/tcp.h>
#include <sys/fanotify.h>
#include <sys/inotify.h>
#include <sys/mount.h>
#include <sys/procfs.h>
#include <sys/resource.h>
#include <sys/signalfd.h>
#endif

#if defined(__SLATE_LIBC_GLIBC)
#if defined(__SLATE_ARCH_X86_64) || defined(__SLATE_ARCH_X86)
#include <sys/pidfd.h>
#endif
#include <sys/rseq.h>
#endif

#define SIZE(name, type) printf("size\t%s\t%zu\n", name, sizeof(type))
#define ALIGN(name, type) printf("align\t%s\t%zu\n", name, _Alignof(type))
#define OFFSET(name, type, field) \
  printf("offset\t%s.%s\t%zu\n", name, #field, offsetof(type, field))
#define MACRO(name) printf("macro\t%s\t%ld\n", #name, (long)(name))
#define PRESENCE(name, value) printf("presence\t%s\t%d\n", name, value)

static void emit_types(void) {
  SIZE("char", char);
  SIZE("short", short);
  SIZE("int", int);
  SIZE("long", long);
  SIZE("long_long", long long);
  SIZE("void_pointer", void *);
  SIZE("size_t", size_t);
  SIZE("ssize_t", ssize_t);
  SIZE("off_t", off_t);
  SIZE("ino_t", ino_t);
  SIZE("dev_t", dev_t);
  SIZE("nlink_t", nlink_t);
  SIZE("time_t", time_t);
  SIZE("clock_t", clock_t);
  SIZE("pid_t", pid_t);
  SIZE("uid_t", uid_t);
  SIZE("gid_t", gid_t);
  SIZE("mode_t", mode_t);
  SIZE("blkcnt_t", blkcnt_t);
  SIZE("blksize_t", blksize_t);
  ALIGN("long", long);
  ALIGN("void_pointer", void *);
  ALIGN("size_t", size_t);
  ALIGN("off_t", off_t);
  ALIGN("time_t", time_t);
}

static void emit_filesystem(void) {
  SIZE("struct_stat", struct stat);
  ALIGN("struct_stat", struct stat);
  OFFSET("struct_stat", struct stat, st_dev);
  OFFSET("struct_stat", struct stat, st_ino);
  OFFSET("struct_stat", struct stat, st_mode);
  OFFSET("struct_stat", struct stat, st_nlink);
  OFFSET("struct_stat", struct stat, st_uid);
  OFFSET("struct_stat", struct stat, st_gid);
  OFFSET("struct_stat", struct stat, st_size);
  OFFSET("struct_stat", struct stat, st_blksize);
  OFFSET("struct_stat", struct stat, st_blocks);
  OFFSET("struct_stat", struct stat, st_atim);
  OFFSET("struct_stat", struct stat, st_mtim);
  OFFSET("struct_stat", struct stat, st_ctim);
  SIZE("struct_dirent", struct dirent);
  ALIGN("struct_dirent", struct dirent);
  OFFSET("struct_dirent", struct dirent, d_ino);
  OFFSET("struct_dirent", struct dirent, d_off);
  OFFSET("struct_dirent", struct dirent, d_reclen);
  OFFSET("struct_dirent", struct dirent, d_name);
  SIZE("struct_statvfs", struct statvfs);
  ALIGN("struct_statvfs", struct statvfs);
  OFFSET("struct_statvfs", struct statvfs, f_bsize);
  OFFSET("struct_statvfs", struct statvfs, f_frsize);
  OFFSET("struct_statvfs", struct statvfs, f_blocks);
  OFFSET("struct_statvfs", struct statvfs, f_files);
}

static void emit_time_signal(void) {
  SIZE("struct_timespec", struct timespec);
  ALIGN("struct_timespec", struct timespec);
  OFFSET("struct_timespec", struct timespec, tv_sec);
  OFFSET("struct_timespec", struct timespec, tv_nsec);
  SIZE("struct_timeval", struct timeval);
  SIZE("struct_tm", struct tm);
  SIZE("sigset_t", sigset_t);
  SIZE("struct_sigaction", struct sigaction);
  SIZE("stack_t", stack_t);
  SIZE("ucontext_t", ucontext_t);
  SIZE("jmp_buf", jmp_buf);
  SIZE("sigjmp_buf", sigjmp_buf);
}

static void emit_network(void) {
  SIZE("struct_sockaddr", struct sockaddr);
  SIZE("struct_sockaddr_storage", struct sockaddr_storage);
  SIZE("struct_sockaddr_in", struct sockaddr_in);
  SIZE("struct_sockaddr_in6", struct sockaddr_in6);
  SIZE("struct_sockaddr_un", struct sockaddr_un);
  SIZE("struct_msghdr", struct msghdr);
  ALIGN("struct_msghdr", struct msghdr);
  SIZE("struct_cmsghdr", struct cmsghdr);
  ALIGN("struct_cmsghdr", struct cmsghdr);
  SIZE("struct_linger", struct linger);
  SIZE("struct_addrinfo", struct addrinfo);
  SIZE("struct_pollfd", struct pollfd);
  SIZE("struct_epoll_event", struct epoll_event);
  OFFSET("struct_sockaddr", struct sockaddr, sa_family);
  OFFSET("struct_sockaddr_in", struct sockaddr_in, sin_family);
  OFFSET("struct_sockaddr_in6", struct sockaddr_in6, sin6_family);
  OFFSET("struct_msghdr", struct msghdr, msg_name);
  OFFSET("struct_msghdr", struct msghdr, msg_namelen);
  OFFSET("struct_msghdr", struct msghdr, msg_iov);
  OFFSET("struct_msghdr", struct msghdr, msg_iovlen);
  OFFSET("struct_msghdr", struct msghdr, msg_control);
  OFFSET("struct_msghdr", struct msghdr, msg_controllen);
  OFFSET("struct_msghdr", struct msghdr, msg_flags);
  OFFSET("struct_cmsghdr", struct cmsghdr, cmsg_len);
  OFFSET("struct_cmsghdr", struct cmsghdr, cmsg_level);
  OFFSET("struct_cmsghdr", struct cmsghdr, cmsg_type);
}

static void emit_extensions(void) {
  SIZE("struct_elf_prstatus", struct elf_prstatus);
  ALIGN("struct_elf_prstatus", struct elf_prstatus);
  OFFSET("struct_elf_prstatus", struct elf_prstatus, pr_utime);
  OFFSET("struct_elf_prstatus", struct elf_prstatus, pr_stime);
  OFFSET("struct_elf_prstatus", struct elf_prstatus, pr_reg);
  OFFSET("struct_elf_prstatus", struct elf_prstatus, pr_fpvalid);
  SIZE("struct_elf_prpsinfo", struct elf_prpsinfo);
  OFFSET("struct_elf_prpsinfo", struct elf_prpsinfo, pr_uid);
  OFFSET("struct_elf_prpsinfo", struct elf_prpsinfo, pr_gid);

  SIZE("struct_rusage", struct rusage);
  ALIGN("struct_rusage", struct rusage);
  OFFSET("struct_rusage", struct rusage, ru_utime);
  OFFSET("struct_rusage", struct rusage, ru_stime);
  OFFSET("struct_rusage", struct rusage, ru_maxrss);
  OFFSET("struct_rusage", struct rusage, ru_nvcsw);
  OFFSET("struct_rusage", struct rusage, ru_nivcsw);
  SIZE("struct_rlimit", struct rlimit);
  OFFSET("struct_rlimit", struct rlimit, rlim_cur);
  OFFSET("struct_rlimit", struct rlimit, rlim_max);

  SIZE("struct_statx_timestamp", struct statx_timestamp);
  ALIGN("struct_statx_timestamp", struct statx_timestamp);
  OFFSET("struct_statx_timestamp", struct statx_timestamp, tv_sec);
  OFFSET("struct_statx_timestamp", struct statx_timestamp, tv_nsec);
  SIZE("struct_statx", struct statx);
  ALIGN("struct_statx", struct statx);
  OFFSET("struct_statx", struct statx, stx_mask);
  OFFSET("struct_statx", struct statx, stx_blksize);
  OFFSET("struct_statx", struct statx, stx_attributes);
  OFFSET("struct_statx", struct statx, stx_nlink);
  OFFSET("struct_statx", struct statx, stx_uid);
  OFFSET("struct_statx", struct statx, stx_gid);
  OFFSET("struct_statx", struct statx, stx_mode);
  OFFSET("struct_statx", struct statx, stx_ino);
  OFFSET("struct_statx", struct statx, stx_size);
  OFFSET("struct_statx", struct statx, stx_blocks);
  OFFSET("struct_statx", struct statx, stx_attributes_mask);
  OFFSET("struct_statx", struct statx, stx_atime);
  OFFSET("struct_statx", struct statx, stx_btime);
  OFFSET("struct_statx", struct statx, stx_ctime);
  OFFSET("struct_statx", struct statx, stx_mtime);
  OFFSET("struct_statx", struct statx, stx_rdev_major);
  OFFSET("struct_statx", struct statx, stx_rdev_minor);
  OFFSET("struct_statx", struct statx, stx_dev_major);
  OFFSET("struct_statx", struct statx, stx_dev_minor);
  OFFSET("struct_statx", struct statx, stx_mnt_id);
  OFFSET("struct_statx", struct statx, stx_dio_mem_align);
  OFFSET("struct_statx", struct statx, stx_dio_offset_align);

  SIZE("struct_fanotify_event_metadata", struct fanotify_event_metadata);
  OFFSET("struct_fanotify_event_metadata", struct fanotify_event_metadata,
         mask);
  OFFSET("struct_fanotify_event_metadata", struct fanotify_event_metadata,
         fd);
  OFFSET("struct_fanotify_event_metadata", struct fanotify_event_metadata,
         pid);
  SIZE("struct_fanotify_event_info_header", struct fanotify_event_info_header);
  OFFSET("struct_fanotify_event_info_header", struct fanotify_event_info_header,
         len);
  SIZE("struct_fanotify_event_info_fid", struct fanotify_event_info_fid);
  OFFSET("struct_fanotify_event_info_fid", struct fanotify_event_info_fid, fsid);
  SIZE("struct_fanotify_response", struct fanotify_response);
  OFFSET("struct_fanotify_response", struct fanotify_response, response);

  SIZE("struct_signalfd_siginfo", struct signalfd_siginfo);
  OFFSET("struct_signalfd_siginfo", struct signalfd_siginfo, ssi_signo);
  OFFSET("struct_signalfd_siginfo", struct signalfd_siginfo, ssi_pid);
  OFFSET("struct_signalfd_siginfo", struct signalfd_siginfo, ssi_utime);
  OFFSET("struct_signalfd_siginfo", struct signalfd_siginfo, ssi_arch);
  SIZE("struct_inotify_event", struct inotify_event);
  OFFSET("struct_inotify_event", struct inotify_event, wd);
  OFFSET("struct_inotify_event", struct inotify_event, mask);
  OFFSET("struct_inotify_event", struct inotify_event, len);

  SIZE("struct_tcp_info", struct tcp_info);
  OFFSET("struct_tcp_info", struct tcp_info, tcpi_state);
  OFFSET("struct_tcp_info", struct tcp_info, tcpi_rto);
#if !defined(__SLATE_ARCH_ARM)
  OFFSET("struct_tcp_info", struct tcp_info, tcpi_pacing_rate);
  OFFSET("struct_tcp_info", struct tcp_info, tcpi_bytes_sent);
#endif
  SIZE("struct_tcp_md5sig", struct tcp_md5sig);
  OFFSET("struct_tcp_md5sig", struct tcp_md5sig, tcpm_addr);
  OFFSET("struct_tcp_md5sig", struct tcp_md5sig, tcpm_key);
  SIZE("struct_tcp_zerocopy_receive", struct tcp_zerocopy_receive);
#if !defined(__SLATE_ARCH_ARM)
  OFFSET("struct_tcp_zerocopy_receive", struct tcp_zerocopy_receive,
         copybuf_address);
#endif

#if defined(__SLATE_LIBC_GLIBC) && defined(MOUNT_ATTR_SIZE_VER0)
  PRESENCE("sys.mount.struct_mount_attr", 1);
  SIZE("struct_mount_attr", struct mount_attr);
  ALIGN("struct_mount_attr", struct mount_attr);
  OFFSET("struct_mount_attr", struct mount_attr, attr_set);
  OFFSET("struct_mount_attr", struct mount_attr, userns_fd);
#elif defined(__SLATE_LIBC_GLIBC)
  PRESENCE("sys.mount.struct_mount_attr", 0);
#endif

#if defined(__SLATE_LIBC_GLIBC)
  PRESENCE("sys.rseq.struct_rseq_cs", 1);
  SIZE("struct_rseq_cs", struct rseq_cs);
  ALIGN("struct_rseq_cs", struct rseq_cs);
  OFFSET("struct_rseq_cs", struct rseq_cs, start_ip);
  OFFSET("struct_rseq_cs", struct rseq_cs, abort_ip);
  PRESENCE("sys.rseq.struct_rseq", 1);
  SIZE("struct_rseq", struct rseq);
  ALIGN("struct_rseq", struct rseq);
  OFFSET("struct_rseq", struct rseq, cpu_id_start);
  OFFSET("struct_rseq", struct rseq, rseq_cs);
  OFFSET("struct_rseq", struct rseq, flags);
#if defined(__SLATE_ARCH_X86_64) || defined(__SLATE_ARCH_X86)
  PRESENCE("sys.pidfd.struct_pidfd_info", 1);
  SIZE("struct_pidfd_info", struct pidfd_info);
  OFFSET("struct_pidfd_info", struct pidfd_info, mask);
  OFFSET("struct_pidfd_info", struct pidfd_info, pid);
  OFFSET("struct_pidfd_info", struct pidfd_info, supported_mask);
#endif
#endif
}

static void emit_registers(void) {
#if defined(__SLATE_ARCH_X86_64)
  PRESENCE("sys.reg.x86_64", 1);
  MACRO(R15);
  MACRO(RBP);
  MACRO(RAX);
  MACRO(ORIG_RAX);
  MACRO(RIP);
  MACRO(EFLAGS);
  MACRO(RSP);
  MACRO(GS);
#elif defined(__SLATE_ARCH_X86)
  PRESENCE("sys.reg.x86", 1);
  MACRO(EBX);
  MACRO(EAX);
  MACRO(ORIG_EAX);
  MACRO(EIP);
  MACRO(EFL);
  MACRO(UESP);
  MACRO(SS);
#endif

#if defined(__SLATE_ARCH_X86_64)
  PRESENCE("sys.user.struct_user_regs_struct", 1);
  SIZE("struct_user_regs_struct", struct user_regs_struct);
  ALIGN("struct_user_regs_struct", struct user_regs_struct);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, r15);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, rbp);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, rax);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, orig_rax);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, rip);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, eflags);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, rsp);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, gs);
  SIZE("struct_user_fpregs_struct", struct user_fpregs_struct);
  ALIGN("struct_user_fpregs_struct", struct user_fpregs_struct);
  OFFSET("struct_user_fpregs_struct", struct user_fpregs_struct, cwd);
  OFFSET("struct_user_fpregs_struct", struct user_fpregs_struct, rip);
  OFFSET("struct_user_fpregs_struct", struct user_fpregs_struct, mxcsr);
  OFFSET("struct_user_fpregs_struct", struct user_fpregs_struct, st_space);
  OFFSET("struct_user_fpregs_struct", struct user_fpregs_struct, xmm_space);
  SIZE("struct_user", struct user);
  OFFSET("struct_user", struct user, regs);
  OFFSET("struct_user", struct user, u_fpvalid);
  OFFSET("struct_user", struct user, i387);
  OFFSET("struct_user", struct user, u_debugreg);
#elif defined(__SLATE_ARCH_X86)
  PRESENCE("sys.user.struct_user_regs_struct", 1);
  SIZE("struct_user_regs_struct", struct user_regs_struct);
  ALIGN("struct_user_regs_struct", struct user_regs_struct);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, ebx);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, eax);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, orig_eax);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, eip);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, eflags);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, esp);
  SIZE("struct_user", struct user);
  OFFSET("struct_user", struct user, regs);
  OFFSET("struct_user", struct user, u_fpvalid);
  OFFSET("struct_user", struct user, i387);
  OFFSET("struct_user", struct user, u_debugreg);
#elif defined(__SLATE_ARCH_AARCH64)
  PRESENCE("sys.user.struct_user_regs_struct", 1);
  SIZE("struct_user_regs_struct", struct user_regs_struct);
  ALIGN("struct_user_regs_struct", struct user_regs_struct);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, regs);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, sp);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, pc);
  OFFSET("struct_user_regs_struct", struct user_regs_struct, pstate);
  SIZE("struct_user_fpsimd_struct", struct user_fpsimd_struct);
  ALIGN("struct_user_fpsimd_struct", struct user_fpsimd_struct);
  OFFSET("struct_user_fpsimd_struct", struct user_fpsimd_struct, vregs);
  OFFSET("struct_user_fpsimd_struct", struct user_fpsimd_struct, fpsr);
  OFFSET("struct_user_fpsimd_struct", struct user_fpsimd_struct, fpcr);
#elif defined(__SLATE_ARCH_ARM)
  PRESENCE("sys.user.struct_user_regs_struct", 1);
  SIZE("struct_user_regs", struct user_regs);
  ALIGN("struct_user_regs", struct user_regs);
  OFFSET("struct_user_regs", struct user_regs, uregs);
  SIZE("struct_user", struct user);
  OFFSET("struct_user", struct user, regs);
  OFFSET("struct_user", struct user, u_fpvalid);
  OFFSET("struct_user", struct user, u_debugreg);
  OFFSET("struct_user", struct user, u_fp);
#elif defined(__SLATE_ARCH_RISCV32) || defined(__SLATE_ARCH_RISCV64)
  PRESENCE("sys.user.struct_user_regs_struct", 0);
#endif
}

static void emit_threads(void) {
  SIZE("pthread_t", pthread_t);
  SIZE("pthread_key_t", pthread_key_t);
  SIZE("pthread_attr_t", pthread_attr_t);
  SIZE("pthread_mutex_t", pthread_mutex_t);
  SIZE("pthread_mutexattr_t", pthread_mutexattr_t);
  SIZE("pthread_cond_t", pthread_cond_t);
  SIZE("pthread_condattr_t", pthread_condattr_t);
  SIZE("pthread_rwlock_t", pthread_rwlock_t);
  SIZE("pthread_rwlockattr_t", pthread_rwlockattr_t);
  SIZE("pthread_once_t", pthread_once_t);
  SIZE("sem_t", sem_t);
  SIZE("struct_sched_param", struct sched_param);
  ALIGN("pthread_t", pthread_t);
  ALIGN("pthread_mutex_t", pthread_mutex_t);
  ALIGN("pthread_cond_t", pthread_cond_t);
  ALIGN("sem_t", sem_t);
}

static void emit_macros(void) {
#ifdef __LP64__
  MACRO(__LP64__);
#endif
#ifdef __ILP32__
  MACRO(__ILP32__);
#endif
#ifdef __SIZEOF_POINTER__
  MACRO(__SIZEOF_POINTER__);
#endif
#ifdef __SIZEOF_LONG__
  MACRO(__SIZEOF_LONG__);
#endif
#ifdef __CHAR_BIT__
  MACRO(__CHAR_BIT__);
#endif
  MACRO(AF_INET);
  MACRO(AF_INET6);
  MACRO(O_CLOEXEC);
  MACRO(MAP_PRIVATE);
  MACRO(SIGRTMIN);
}

int main(void) {
  printf("schema\tversion\t1\n");
  emit_types();
  emit_filesystem();
  emit_time_signal();
  emit_network();
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  emit_extensions();
#endif
  emit_registers();
  emit_threads();
  emit_macros();
  return 0;
}
