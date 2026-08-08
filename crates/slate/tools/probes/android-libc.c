#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

struct stat slate_android_stat;
struct dirent slate_android_dirent;
struct sockaddr_storage slate_android_sockaddr_storage;
pthread_mutex_t slate_android_mutex = PTHREAD_MUTEX_INITIALIZER;
sigset_t slate_android_sigset;
struct timespec slate_android_timespec;

int slate_android_export(const char *path) {
  struct stat value;
  return stat(path, &value) + clock_gettime(CLOCK_REALTIME, &slate_android_timespec);
}

int slate_android_imports(void) {
  return printf("%d\n", errno);
}
