#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <langinfo.h>
#include <locale.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/ucontext.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#include <xlocale.h>

_Static_assert(sizeof(pthread_t) == 8, "pthread_t");
_Static_assert(sizeof(pthread_key_t) == 8, "pthread_key_t");
_Static_assert(sizeof(pthread_attr_t) == 64, "pthread_attr_t");
_Static_assert(sizeof(pthread_mutex_t) == 64, "pthread_mutex_t");
_Static_assert(sizeof(pthread_mutexattr_t) == 16, "pthread_mutexattr_t");
_Static_assert(sizeof(pthread_cond_t) == 48, "pthread_cond_t");
_Static_assert(sizeof(pthread_condattr_t) == 16, "pthread_condattr_t");
_Static_assert(sizeof(pthread_once_t) == 16, "pthread_once_t");
_Static_assert(sizeof(pthread_rwlock_t) == 200, "pthread_rwlock_t");
_Static_assert(sizeof(pthread_rwlockattr_t) == 24, "pthread_rwlockattr_t");
_Static_assert(sizeof(sem_t) == 4, "sem_t");
_Static_assert(sizeof(struct sched_param) == 8, "sched_param");
_Static_assert(_Alignof(pthread_t) == 8, "pthread_t alignment");
_Static_assert(_Alignof(pthread_key_t) == 8, "pthread_key_t alignment");
_Static_assert(_Alignof(pthread_attr_t) == 8, "pthread_attr_t alignment");
_Static_assert(_Alignof(pthread_mutex_t) == 8, "pthread_mutex_t alignment");
_Static_assert(_Alignof(pthread_mutexattr_t) == 8,
               "pthread_mutexattr_t alignment");
_Static_assert(_Alignof(pthread_cond_t) == 8, "pthread_cond_t alignment");
_Static_assert(_Alignof(pthread_condattr_t) == 8,
               "pthread_condattr_t alignment");
_Static_assert(_Alignof(pthread_once_t) == 8, "pthread_once_t alignment");
_Static_assert(_Alignof(pthread_rwlock_t) == 8, "pthread_rwlock_t alignment");
_Static_assert(_Alignof(pthread_rwlockattr_t) == 8,
               "pthread_rwlockattr_t alignment");
_Static_assert(_Alignof(sem_t) == 4, "sem_t alignment");
_Static_assert(_Alignof(struct sched_param) == 4, "sched_param alignment");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&pthread_setcancelstate), int (*)(int, int *)),
               "pthread_setcancelstate signature");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&pthread_cond_timedwait),
                   int (*)(pthread_cond_t *__restrict,
                           pthread_mutex_t *__restrict,
                           const struct timespec *__restrict)),
               "pthread_cond_timedwait signature");
_Static_assert(sizeof(struct sockaddr) == 16, "sockaddr");
_Static_assert(offsetof(struct sockaddr, sa_family) == 1, "sa_family");
_Static_assert(sizeof(struct sockaddr_storage) == 128, "sockaddr_storage");
_Static_assert(_Alignof(struct sockaddr_storage) == 8,
               "sockaddr_storage alignment");
_Static_assert(sizeof(struct sockaddr_in) == 16, "sockaddr_in");
_Static_assert(sizeof(struct sockaddr_in6) == 28, "sockaddr_in6");
_Static_assert(sizeof(struct sockaddr_un) == 106, "sockaddr_un");
_Static_assert(sizeof(struct msghdr) == 48, "msghdr");
_Static_assert(sizeof(struct cmsghdr) == 12, "cmsghdr");
_Static_assert(CMSG_LEN(1) == 13, "CMSG_LEN");
_Static_assert(CMSG_SPACE(1) == 16, "CMSG_SPACE");
_Static_assert(sizeof(struct addrinfo) == 48, "addrinfo");
_Static_assert(offsetof(struct addrinfo, ai_canonname) == 24,
               "ai_canonname");
_Static_assert(offsetof(struct addrinfo, ai_addr) == 32, "ai_addr");
_Static_assert(sizeof(struct ifaddrs) == 56, "ifaddrs");
_Static_assert(AF_INET6 == 30, "AF_INET6");
_Static_assert(SOL_SOCKET == 0xffff, "SOL_SOCKET");
_Static_assert(AI_NUMERICSERV == 0x1000, "AI_NUMERICSERV");
_Static_assert(NI_NUMERICSCOPE == 0x100, "NI_NUMERICSCOPE");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&recvfrom),
                   ssize_t (*)(int, void *, size_t, int,
                               struct sockaddr *__restrict,
                               socklen_t *__restrict)),
               "recvfrom signature");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&getnameinfo),
                   int (*)(const struct sockaddr *__restrict, socklen_t,
                           char *__restrict, socklen_t, char *__restrict,
                           socklen_t, int)),
               "getnameinfo signature");

struct slate_macos_stdio_locale_layouts {
  FILE      stream;
  fpos_t    position;
  mbstate_t conversion;
  struct lconv numeric;
  locale_t  locale;
  wint_t    wide;
  wctype_t  classification;
  wctrans_t transformation;
};

struct slate_macos_filesystem_layouts {
  struct stat    file_status;
  struct dirent  directory_entry;
  struct statvfs filesystem_status;
};

struct slate_macos_time_signal_layouts {
  struct timespec  precise_time;
  struct timeval   wall_time;
  struct timezone  zone;
  struct tm        calendar;
  struct itimerval interval_timer;
  struct sigevent  event;
  siginfo_t        signal_info;
  struct sigaction action;
  stack_t          alternate_stack;
  ucontext_t       context;
  jmp_buf          jump;
  sigjmp_buf       signal_jump;
};

struct slate_macos_pthread_layouts {
  pthread_t            thread;
  pthread_key_t        key;
  pthread_attr_t       attribute;
  pthread_mutex_t      mutex;
  pthread_mutexattr_t  mutex_attribute;
  pthread_cond_t       condition;
  pthread_condattr_t   condition_attribute;
  pthread_once_t       once;
  pthread_rwlock_t     rwlock;
  pthread_rwlockattr_t rwlock_attribute;
  sem_t                semaphore;
  struct sched_param   scheduling;
};

struct slate_macos_network_layouts {
  struct sockaddr         address;
  struct sockaddr_storage storage;
  struct sockaddr_in      ipv4;
  struct sockaddr_in6     ipv6;
  struct sockaddr_un      local;
  struct msghdr           message;
  struct cmsghdr          control;
  struct linger           linger;
  struct addrinfo         resolution;
  struct hostent          host;
  struct servent          service;
  struct protoent         protocol;
  struct ifaddrs          interface;
};

pthread_mutex_t  slate_macos_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   slate_macos_condition = PTHREAD_COND_INITIALIZER;
pthread_once_t   slate_macos_once = PTHREAD_ONCE_INIT;
pthread_rwlock_t slate_macos_rwlock = PTHREAD_RWLOCK_INITIALIZER;

FILE *slate_macos_open(const char *path) { return fopen(path, "r"); }

int slate_macos_errno(void) { return errno; }

FILE *slate_macos_stdin(void) { return stdin; }

int slate_macos_classify(wint_t value, locale_t locale) {
  return iswctype_l(value, wctype("alpha"), locale);
}

int slate_macos_export(void) { return LC_ALL + CODESET; }

int slate_macos_stat(const char *path, struct stat *out) {
  return stat(path, out);
}

struct dirent *slate_macos_readdir(DIR *dir) { return readdir(dir); }

int slate_macos_open_read_only(const char *path) {
  return open(path, O_RDONLY);
}

int slate_macos_clock(struct timespec *value) {
  return clock_gettime(CLOCK_MONOTONIC, value);
}

int slate_macos_set_timer(const struct itimerval *value) {
  return setitimer(ITIMER_REAL, value, 0);
}

int slate_macos_sigmask(sigset_t *mask) {
  return sigprocmask(SIG_BLOCK, mask, 0);
}

int slate_macos_altstack(stack_t *stack) { return sigaltstack(stack, 0); }

int slate_macos_pthread_create(pthread_t *thread, void *(*entry)(void *),
                               void *argument) {
  return pthread_create(thread, 0, entry, argument);
}

int slate_macos_pthread_timedwait(const struct timespec *deadline) {
  return pthread_cond_timedwait(&slate_macos_condition, &slate_macos_mutex,
                                deadline);
}

int slate_macos_pthread_once(void (*routine)(void)) {
  return pthread_once(&slate_macos_once, routine);
}

int slate_macos_semaphore(sem_t *semaphore) { return sem_post(semaphore); }

int slate_macos_sched_yield(void) { return sched_yield(); }

int slate_macos_socket(struct sockaddr_in *address) {
  int descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  return descriptor < 0
             ? descriptor
             : bind(descriptor, (const struct sockaddr *)address,
                    sizeof(*address));
}

int slate_macos_resolve(const char *node, struct addrinfo **result) {
  return getaddrinfo(node, "443", 0, result);
}
