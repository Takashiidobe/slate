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
#include <termios.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

#define SIZE(name, type) printf("size\t%s\t%zu\n", name, sizeof(type))
#define ALIGN(name, type) printf("align\t%s\t%zu\n", name, _Alignof(type))
#define OFFSET(name, type, field) \
  printf("offset\t%s.%s\t%zu\n", name, #field, offsetof(type, field))
#define MACRO(name) printf("macro\t%s\t%ld\n", #name, (long)(name))

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
  SIZE("struct_cmsghdr", struct cmsghdr);
  SIZE("struct_linger", struct linger);
  SIZE("struct_addrinfo", struct addrinfo);
  SIZE("struct_pollfd", struct pollfd);
  SIZE("struct_epoll_event", struct epoll_event);
  OFFSET("struct_sockaddr", struct sockaddr, sa_family);
  OFFSET("struct_sockaddr_in", struct sockaddr_in, sin_family);
  OFFSET("struct_sockaddr_in6", struct sockaddr_in6, sin6_family);
  OFFSET("struct_msghdr", struct msghdr, msg_name);
  OFFSET("struct_msghdr", struct msghdr, msg_iov);
  OFFSET("struct_msghdr", struct msghdr, msg_control);
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
  emit_threads();
  emit_macros();
  return 0;
}
