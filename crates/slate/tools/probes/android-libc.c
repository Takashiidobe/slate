#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

struct stat slate_android_stat;
struct stat64 slate_android_stat64;
struct dirent slate_android_dirent;
struct dirent64 slate_android_dirent64;
struct statvfs slate_android_statvfs;
struct utimbuf slate_android_utimbuf;
struct sockaddr_storage slate_android_sockaddr_storage;
pthread_mutex_t slate_android_mutex = PTHREAD_MUTEX_INITIALIZER;
sigset_t slate_android_sigset;
struct timespec slate_android_timespec;

int slate_android_export(const char *path) {
  struct stat value;
  return stat(path, &value) + clock_gettime(CLOCK_REALTIME, &slate_android_timespec);
}

int slate_android_filesystem(const char *path, int fd) {
  struct statvfs vfs;
  struct utimbuf times;
  return statvfs(path, &vfs) + fstatvfs(fd, &vfs) + openat(fd, path, O_RDWR | O_CREAT, 0644) +
         utime(path, &times) + fcntl(fd, F_GETFL);
}

int slate_android_imports(void) {
  return printf("%d\n", errno);
}
