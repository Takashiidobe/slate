#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/event.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

struct stat slate_freebsd_stat;
struct dirent slate_freebsd_dirent;
struct sockaddr_storage slate_freebsd_sockaddr_storage;
struct kevent slate_freebsd_kevent;
pthread_mutex_t slate_freebsd_mutex = PTHREAD_MUTEX_INITIALIZER;
sigset_t slate_freebsd_sigset;
struct timespec slate_freebsd_timespec;

int slate_freebsd_export(const char *path) {
  struct stat value;
  return stat(path, &value) +
         clock_gettime(CLOCK_REALTIME, &slate_freebsd_timespec) + kqueue();
}

int slate_freebsd_imports(void) {
  return errno + (stdin != NULL) + (stdout != NULL) + (stderr != NULL);
}
