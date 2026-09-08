#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#define _GNU_SOURCE
#endif
#if defined(__SLATE_LIBC_FREEBSD)
#define _BSD_SOURCE
#endif

#include <arpa/inet.h>
#if defined(__SLATE_LIBC_MUSL) ||                                              \
    (defined(__SLATE_LIBC_GLIBC) &&                                            \
     (defined(__SLATE_ARCH_X86_64) || defined(__SLATE_ARCH_X86)))
#include <crypt.h>
#define SLATE_PROBE_HAVE_CRYPT 1
#endif
#include <aio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <getopt.h>
#include <glob.h>
#include <grp.h>
#include <ifaddrs.h>
#include <langinfo.h>
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#include <mntent.h>
#endif
#include <mqueue.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <sched.h>
#include <search.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/auxv.h>
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#include <sys/epoll.h>
#endif
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <sys/random.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#include <sys/sysinfo.h>
#endif
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/user.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <termios.h>
#include <threads.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include <utime.h>
#include <utmpx.h>
#include <wchar.h>
#include <wordexp.h>

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

#define SIZE(name, type)  printf("size\t%s\t%zu\n", name, sizeof(type))
#define ALIGN(name, type) printf("align\t%s\t%zu\n", name, _Alignof(type))
#define OFFSET(name, type, field)                                              \
  printf("offset\t%s.%s\t%zu\n", name, #field, offsetof(type, field))
#define MACRO(name)              printf("macro\t%s\t%ld\n", #name, (long)(name))
#define MACRO_VALUE(label, expr) printf("macro\t%s\t%ld\n", label, (long)(expr))
#define PRESENCE(name, value)    printf("presence\t%s\t%d\n", name, value)

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
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  SIZE("ucontext_t", ucontext_t);
#endif
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
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  SIZE("struct_epoll_event", struct epoll_event);
#endif
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

  SIZE("struct_ifaddrs", struct ifaddrs);
  ALIGN("struct_ifaddrs", struct ifaddrs);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_next);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_name);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_flags);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_addr);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_netmask);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_dstaddr);
  OFFSET("struct_ifaddrs", struct ifaddrs, ifa_data);
}

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
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

  SIZE("struct_sysinfo", struct sysinfo);
  ALIGN("struct_sysinfo", struct sysinfo);
  OFFSET("struct_sysinfo", struct sysinfo, uptime);
  OFFSET("struct_sysinfo", struct sysinfo, loads);
  OFFSET("struct_sysinfo", struct sysinfo, totalram);
  OFFSET("struct_sysinfo", struct sysinfo, freeram);
  OFFSET("struct_sysinfo", struct sysinfo, sharedram);
  OFFSET("struct_sysinfo", struct sysinfo, bufferram);
  OFFSET("struct_sysinfo", struct sysinfo, totalswap);
  OFFSET("struct_sysinfo", struct sysinfo, freeswap);
  OFFSET("struct_sysinfo", struct sysinfo, procs);
  OFFSET("struct_sysinfo", struct sysinfo, pad);
  OFFSET("struct_sysinfo", struct sysinfo, totalhigh);
  OFFSET("struct_sysinfo", struct sysinfo, freehigh);
  OFFSET("struct_sysinfo", struct sysinfo, mem_unit);

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
  OFFSET("struct_fanotify_event_metadata", struct fanotify_event_metadata, fd);
  OFFSET("struct_fanotify_event_metadata", struct fanotify_event_metadata, pid);
  SIZE("struct_fanotify_event_info_header", struct fanotify_event_info_header);
  OFFSET("struct_fanotify_event_info_header", struct fanotify_event_info_header,
         len);
  SIZE("struct_fanotify_event_info_fid", struct fanotify_event_info_fid);
  OFFSET("struct_fanotify_event_info_fid", struct fanotify_event_info_fid,
         fsid);
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

  SIZE("struct_ipc_perm", struct ipc_perm);
  ALIGN("struct_ipc_perm", struct ipc_perm);
  OFFSET("struct_ipc_perm", struct ipc_perm, __key);
  OFFSET("struct_ipc_perm", struct ipc_perm, uid);
  OFFSET("struct_ipc_perm", struct ipc_perm, gid);
  OFFSET("struct_ipc_perm", struct ipc_perm, cuid);
  OFFSET("struct_ipc_perm", struct ipc_perm, cgid);
  OFFSET("struct_ipc_perm", struct ipc_perm, mode);
  OFFSET("struct_ipc_perm", struct ipc_perm, __seq);

  SIZE("struct_shmid_ds", struct shmid_ds);
  ALIGN("struct_shmid_ds", struct shmid_ds);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_perm);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_segsz);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_atime);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_dtime);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_ctime);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_cpid);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_lpid);
  OFFSET("struct_shmid_ds", struct shmid_ds, shm_nattch);

  SIZE("struct_msqid_ds", struct msqid_ds);
  ALIGN("struct_msqid_ds", struct msqid_ds);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_perm);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_stime);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_rtime);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_ctime);
  OFFSET("struct_msqid_ds", struct msqid_ds, __msg_cbytes);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_qnum);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_qbytes);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_lspid);
  OFFSET("struct_msqid_ds", struct msqid_ds, msg_lrpid);

  SIZE("struct_semid_ds", struct semid_ds);
  ALIGN("struct_semid_ds", struct semid_ds);
  OFFSET("struct_semid_ds", struct semid_ds, sem_perm);
  OFFSET("struct_semid_ds", struct semid_ds, sem_otime);
  OFFSET("struct_semid_ds", struct semid_ds, sem_ctime);
  OFFSET("struct_semid_ds", struct semid_ds, sem_nsems);
}
#endif

static void emit_regex(void) {
  SIZE("regex_t", regex_t);
  ALIGN("regex_t", regex_t);
  OFFSET("regex_t", regex_t, re_nsub);
  SIZE("regmatch_t", regmatch_t);
  ALIGN("regmatch_t", regmatch_t);
  OFFSET("regmatch_t", regmatch_t, rm_so);
  OFFSET("regmatch_t", regmatch_t, rm_eo);
}

static void emit_glob_wordexp(void) {
  SIZE("glob_t", glob_t);
  ALIGN("glob_t", glob_t);
  OFFSET("glob_t", glob_t, gl_pathc);
  OFFSET("glob_t", glob_t, gl_pathv);
  OFFSET("glob_t", glob_t, gl_offs);

  SIZE("wordexp_t", wordexp_t);
  ALIGN("wordexp_t", wordexp_t);
  OFFSET("wordexp_t", wordexp_t, we_wordc);
  OFFSET("wordexp_t", wordexp_t, we_wordv);
  OFFSET("wordexp_t", wordexp_t, we_offs);

  MACRO(GLOB_ERR);
  MACRO(GLOB_MARK);
  MACRO(GLOB_NOSORT);
  MACRO(GLOB_DOOFFS);
  MACRO(GLOB_NOCHECK);
  MACRO(GLOB_APPEND);
  MACRO(GLOB_NOESCAPE);
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  MACRO(GLOB_PERIOD);
#endif
  MACRO(GLOB_TILDE);
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  MACRO(GLOB_TILDE_CHECK);
#endif
  MACRO(GLOB_NOSPACE);
  MACRO(GLOB_ABORTED);
  MACRO(GLOB_NOMATCH);
  MACRO(GLOB_NOSYS);
#if defined(__SLATE_LIBC_GLIBC)
  PRESENCE("glob.GLOB_MAGCHAR", 1);
  MACRO(GLOB_MAGCHAR);
  MACRO(GLOB_ALTDIRFUNC);
  MACRO(GLOB_BRACE);
  MACRO(GLOB_NOMAGIC);
  MACRO(GLOB_ONLYDIR);
#else
  PRESENCE("glob.GLOB_MAGCHAR", 0);
#endif

  MACRO(FNM_PATHNAME);
  MACRO(FNM_NOESCAPE);
  MACRO(FNM_PERIOD);
  MACRO(FNM_LEADING_DIR);
  MACRO(FNM_CASEFOLD);
  MACRO(FNM_FILE_NAME);
  MACRO(FNM_NOMATCH);
  MACRO(FNM_NOSYS);
#if defined(__SLATE_LIBC_GLIBC)
  PRESENCE("fnmatch.FNM_EXTMATCH", 1);
  MACRO(FNM_EXTMATCH);
#else
  PRESENCE("fnmatch.FNM_EXTMATCH", 0);
#endif

  MACRO(WRDE_DOOFFS);
  MACRO(WRDE_APPEND);
  MACRO(WRDE_NOCMD);
  MACRO(WRDE_REUSE);
  MACRO(WRDE_SHOWERR);
  MACRO(WRDE_UNDEF);
  MACRO(WRDE_NOSPACE);
  MACRO(WRDE_BADCHAR);
  MACRO(WRDE_BADVAL);
  MACRO(WRDE_CMDSUB);
  MACRO(WRDE_SYNTAX);
  MACRO(WRDE_NOSYS);
}

static void emit_accounts(void) {
  SIZE("struct_passwd", struct passwd);
  ALIGN("struct_passwd", struct passwd);
  OFFSET("struct_passwd", struct passwd, pw_name);
  OFFSET("struct_passwd", struct passwd, pw_passwd);
  OFFSET("struct_passwd", struct passwd, pw_uid);
  OFFSET("struct_passwd", struct passwd, pw_gid);
  OFFSET("struct_passwd", struct passwd, pw_gecos);
  OFFSET("struct_passwd", struct passwd, pw_dir);
  OFFSET("struct_passwd", struct passwd, pw_shell);

  SIZE("struct_group", struct group);
  ALIGN("struct_group", struct group);
  OFFSET("struct_group", struct group, gr_name);
  OFFSET("struct_group", struct group, gr_passwd);
  OFFSET("struct_group", struct group, gr_gid);
  OFFSET("struct_group", struct group, gr_mem);

  SIZE("struct_utmpx", struct utmpx);
  ALIGN("struct_utmpx", struct utmpx);
  OFFSET("struct_utmpx", struct utmpx, ut_type);
  OFFSET("struct_utmpx", struct utmpx, ut_pid);
  OFFSET("struct_utmpx", struct utmpx, ut_line);
  OFFSET("struct_utmpx", struct utmpx, ut_id);
  OFFSET("struct_utmpx", struct utmpx, ut_user);
  OFFSET("struct_utmpx", struct utmpx, ut_host);
  OFFSET("struct_utmpx", struct utmpx, ut_tv);
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  OFFSET("struct_utmpx", struct utmpx, ut_exit);
  OFFSET("struct_utmpx", struct utmpx, ut_session);
  OFFSET("struct_utmpx", struct utmpx, ut_addr_v6);
#endif
}

static void emit_misc(void) {
  SIZE("struct_utimbuf", struct utimbuf);
  ALIGN("struct_utimbuf", struct utimbuf);
  OFFSET("struct_utimbuf", struct utimbuf, actime);
  OFFSET("struct_utimbuf", struct utimbuf, modtime);

#if defined(SLATE_PROBE_HAVE_CRYPT)
  PRESENCE("crypt.struct_crypt_data", 1);
  SIZE("struct_crypt_data", struct crypt_data);
  ALIGN("struct_crypt_data", struct crypt_data);
#endif

  SIZE("thrd_t", thrd_t);
  ALIGN("thrd_t", thrd_t);
  SIZE("tss_t", tss_t);
  ALIGN("tss_t", tss_t);
  SIZE("mtx_t", mtx_t);
  ALIGN("mtx_t", mtx_t);
  SIZE("cnd_t", cnd_t);
  ALIGN("cnd_t", cnd_t);
  SIZE("once_flag", once_flag);
  ALIGN("once_flag", once_flag);
}

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
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
#endif

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
static void emit_ucontext(void) {
  ALIGN("ucontext_t", ucontext_t);
  OFFSET("ucontext_t", ucontext_t, uc_flags);
  OFFSET("ucontext_t", ucontext_t, uc_link);
  OFFSET("ucontext_t", ucontext_t, uc_stack);
  OFFSET("ucontext_t", ucontext_t, uc_mcontext);
  OFFSET("ucontext_t", ucontext_t, uc_sigmask);
  SIZE("mcontext_t", mcontext_t);
  ALIGN("mcontext_t", mcontext_t);

#if defined(__SLATE_ARCH_X86_64)
  PRESENCE("sys.ucontext.x86_64", 1);
  MACRO(REG_R8);
  MACRO(REG_R15);
  MACRO(REG_RDI);
  MACRO(REG_RBP);
  MACRO(REG_RBX);
  MACRO(REG_RAX);
  MACRO(REG_RCX);
  MACRO(REG_RSP);
  MACRO(REG_RIP);
  MACRO(REG_EFL);
  MACRO(REG_CSGSFS);
  MACRO(REG_ERR);
  MACRO(REG_TRAPNO);
  MACRO(REG_OLDMASK);
  MACRO(REG_CR2);
  OFFSET("mcontext_t", mcontext_t, gregs);
  OFFSET("mcontext_t", mcontext_t, fpregs);
#elif defined(__SLATE_ARCH_X86)
  PRESENCE("sys.ucontext.x86", 1);
  MACRO(REG_GS);
  MACRO(REG_EDI);
  MACRO(REG_EBP);
  MACRO(REG_EBX);
  MACRO(REG_EDX);
  MACRO(REG_ECX);
  MACRO(REG_EAX);
  MACRO(REG_TRAPNO);
  MACRO(REG_ERR);
  MACRO(REG_EIP);
  MACRO(REG_CS);
  MACRO(REG_EFL);
  MACRO(REG_UESP);
  MACRO(REG_SS);
  OFFSET("mcontext_t", mcontext_t, gregs);
  OFFSET("mcontext_t", mcontext_t, fpregs);
  OFFSET("mcontext_t", mcontext_t, oldmask);
  OFFSET("mcontext_t", mcontext_t, cr2);
#elif defined(__SLATE_ARCH_AARCH64)
  PRESENCE("sys.ucontext.aarch64", 1);
  OFFSET("mcontext_t", mcontext_t, fault_address);
  OFFSET("mcontext_t", mcontext_t, regs);
  OFFSET("mcontext_t", mcontext_t, sp);
  OFFSET("mcontext_t", mcontext_t, pc);
  OFFSET("mcontext_t", mcontext_t, pstate);
#elif defined(__SLATE_ARCH_ARM)
  PRESENCE("sys.ucontext.arm", 1);
  OFFSET("mcontext_t", mcontext_t, trap_no);
  OFFSET("mcontext_t", mcontext_t, error_code);
  OFFSET("mcontext_t", mcontext_t, oldmask);
  OFFSET("mcontext_t", mcontext_t, arm_r0);
  OFFSET("mcontext_t", mcontext_t, arm_fp);
  OFFSET("mcontext_t", mcontext_t, arm_sp);
  OFFSET("mcontext_t", mcontext_t, arm_lr);
  OFFSET("mcontext_t", mcontext_t, arm_pc);
  OFFSET("mcontext_t", mcontext_t, arm_cpsr);
  OFFSET("mcontext_t", mcontext_t, fault_address);
#endif
}
#endif

static void emit_hwcap(void) {
#if defined(__SLATE_ARCH_AARCH64)
  PRESENCE("sys.auxv.aarch64", 1);
  MACRO(HWCAP_FP);
  MACRO(HWCAP_ASIMD);
  MACRO(HWCAP_EVTSTRM);
  MACRO(HWCAP_AES);
  MACRO(HWCAP_PMULL);
  MACRO(HWCAP_SHA1);
  MACRO(HWCAP_SHA2);
  MACRO(HWCAP_CRC32);
  MACRO(HWCAP_ATOMICS);
  MACRO(HWCAP_SVE);
  MACRO(HWCAP_PACA);
  MACRO(HWCAP_PACG);
  MACRO(HWCAP2_SVE2);
  MACRO(HWCAP2_BTI);
  MACRO(HWCAP2_MTE);
#elif defined(__SLATE_ARCH_ARM)
  PRESENCE("sys.auxv.arm", 1);
  MACRO(HWCAP_ARM_SWP);
  MACRO(HWCAP_ARM_THUMB);
  MACRO(HWCAP_ARM_VFP);
  MACRO(HWCAP_ARM_NEON);
  MACRO(HWCAP_ARM_VFPv3);
  MACRO(HWCAP_ARM_TLS);
  MACRO(HWCAP_ARM_VFPv4);
  MACRO(HWCAP_ARM_IDIVA);
  MACRO(HWCAP_ARM_IDIVT);
  MACRO(HWCAP_ARM_LPAE);
  MACRO(HWCAP_ARM_EVTSTRM);
  MACRO(HWCAP2_AES);
  MACRO(HWCAP2_PMULL);
  MACRO(HWCAP2_SHA1);
  MACRO(HWCAP2_SHA2);
  MACRO(HWCAP2_CRC32);
#if defined(__SLATE_LIBC_MUSL)
  MACRO(HWCAP_NEON);
  MACRO(HWCAP_VFPv3);
  MACRO(HWCAP_IDIV);
#endif
#endif
}

static void emit_odds_and_ends(void) {
  SIZE("struct_option", struct option);
  ALIGN("struct_option", struct option);
  OFFSET("struct_option", struct option, name);
  OFFSET("struct_option", struct option, has_arg);
  OFFSET("struct_option", struct option, flag);
  OFFSET("struct_option", struct option, val);

  SIZE("struct_timeb", struct timeb);
  ALIGN("struct_timeb", struct timeb);
  OFFSET("struct_timeb", struct timeb, time);
  OFFSET("struct_timeb", struct timeb, millitm);
  OFFSET("struct_timeb", struct timeb, timezone);
  OFFSET("struct_timeb", struct timeb, dstflag);

  MACRO(NBBY);
  MACRO(MAXPATHLEN);
  MACRO(MAXHOSTNAMELEN);
  MACRO(MAXSYMLINKS);
  MACRO(NOFILE);
  MACRO(DEV_BSIZE);
  MACRO(NOGROUP);

  MACRO(GRND_NONBLOCK);
  MACRO(GRND_RANDOM);
  MACRO(GRND_INSECURE);

  SIZE("struct_utsname", struct utsname);
  ALIGN("struct_utsname", struct utsname);
  OFFSET("struct_utsname", struct utsname, sysname);
  OFFSET("struct_utsname", struct utsname, nodename);
  OFFSET("struct_utsname", struct utsname, release);
  OFFSET("struct_utsname", struct utsname, version);
  OFFSET("struct_utsname", struct utsname, machine);
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  OFFSET("struct_utsname", struct utsname, domainname);
#endif

  SIZE("struct_tms", struct tms);
  ALIGN("struct_tms", struct tms);
  OFFSET("struct_tms", struct tms, tms_utime);
  OFFSET("struct_tms", struct tms, tms_stime);
  OFFSET("struct_tms", struct tms, tms_cutime);
  OFFSET("struct_tms", struct tms, tms_cstime);

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  SIZE("struct_mntent", struct mntent);
  ALIGN("struct_mntent", struct mntent);
  OFFSET("struct_mntent", struct mntent, mnt_fsname);
  OFFSET("struct_mntent", struct mntent, mnt_dir);
  OFFSET("struct_mntent", struct mntent, mnt_type);
  OFFSET("struct_mntent", struct mntent, mnt_opts);
  OFFSET("struct_mntent", struct mntent, mnt_freq);
  OFFSET("struct_mntent", struct mntent, mnt_passno);
#endif

  SIZE("ENTRY", ENTRY);
  ALIGN("ENTRY", ENTRY);
  OFFSET("ENTRY", ENTRY, key);
  OFFSET("ENTRY", ENTRY, data);
}

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
static void emit_aio_mqueue(void) {
  SIZE("struct_aiocb", struct aiocb);
  ALIGN("struct_aiocb", struct aiocb);
  OFFSET("struct_aiocb", struct aiocb, aio_fildes);
  OFFSET("struct_aiocb", struct aiocb, aio_lio_opcode);
  OFFSET("struct_aiocb", struct aiocb, aio_reqprio);
  OFFSET("struct_aiocb", struct aiocb, aio_buf);
  OFFSET("struct_aiocb", struct aiocb, aio_nbytes);
  OFFSET("struct_aiocb", struct aiocb, aio_sigevent);
  OFFSET("struct_aiocb", struct aiocb, aio_offset);

  SIZE("struct_mq_attr", struct mq_attr);
  ALIGN("struct_mq_attr", struct mq_attr);
  OFFSET("struct_mq_attr", struct mq_attr, mq_flags);
  OFFSET("struct_mq_attr", struct mq_attr, mq_maxmsg);
  OFFSET("struct_mq_attr", struct mq_attr, mq_msgsize);
  OFFSET("struct_mq_attr", struct mq_attr, mq_curmsgs);
}

static void emit_wait_mman_langinfo(void) {
  MACRO(WNOHANG);
  MACRO(WUNTRACED);
  MACRO_VALUE("WIFEXITED(0)", WIFEXITED(0));
  MACRO_VALUE("WIFEXITED(256)", WIFEXITED(256));
  MACRO_VALUE("WEXITSTATUS(256)", WEXITSTATUS(256));
  MACRO_VALUE("WIFSIGNALED(9)", WIFSIGNALED(9));
  MACRO_VALUE("WTERMSIG(9)", WTERMSIG(9));
  MACRO_VALUE("WIFSTOPPED(0x137f)", WIFSTOPPED(0x137f));
  MACRO_VALUE("WSTOPSIG(0x137f)", WSTOPSIG(0x137f));
  MACRO_VALUE("WCOREDUMP(0x80)", WCOREDUMP(0x80));

  MACRO(MAP_SHARED);
  MACRO(MAP_FIXED);
  MACRO(MAP_ANONYMOUS);
  MACRO(MAP_NORESERVE);
  MACRO(MAP_GROWSDOWN);
  MACRO(MAP_DENYWRITE);
  MACRO(MAP_EXECUTABLE);
  MACRO(MAP_LOCKED);
  MACRO(MAP_POPULATE);
  MACRO(MAP_NONBLOCK);
  MACRO(MAP_STACK);
  MACRO(MAP_HUGETLB);
  MACRO(PROT_NONE);
  MACRO(PROT_READ);
  MACRO(PROT_WRITE);
  MACRO(PROT_EXEC);
  MACRO(PROT_GROWSDOWN);
  MACRO(PROT_GROWSUP);

  MACRO(CODESET);
  MACRO(D_T_FMT);
  MACRO(D_FMT);
  MACRO(T_FMT);
  MACRO(T_FMT_AMPM);
  MACRO(ABDAY_1);
  MACRO(DAY_1);
  MACRO(ABMON_1);
  MACRO(MON_1);
  MACRO(AM_STR);
  MACRO(PM_STR);
  MACRO(RADIXCHAR);
  MACRO(THOUSEP);
  MACRO(YESEXPR);
  MACRO(NOEXPR);
  MACRO(ERA);
  MACRO(CRNCYSTR);
}
#endif

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
  SIZE("pthread_barrier_t", pthread_barrier_t);
  SIZE("pthread_barrierattr_t", pthread_barrierattr_t);
  SIZE("sem_t", sem_t);
  SIZE("struct_sched_param", struct sched_param);
  ALIGN("pthread_t", pthread_t);
  ALIGN("pthread_mutex_t", pthread_mutex_t);
  ALIGN("pthread_cond_t", pthread_cond_t);
  ALIGN("pthread_barrier_t", pthread_barrier_t);
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
  emit_ucontext();
#endif
  emit_regex();
  emit_glob_wordexp();
  emit_accounts();
  emit_misc();
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  emit_registers();
#endif
  emit_hwcap();
  emit_odds_and_ends();
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
  emit_aio_mqueue();
  emit_wait_mman_langinfo();
#endif
  emit_threads();
  emit_macros();
  return 0;
}
