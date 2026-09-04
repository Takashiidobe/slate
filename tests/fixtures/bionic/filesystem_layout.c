#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <utime.h>

#if defined(__SLATE_ARCH_X86_64)
_Static_assert(sizeof(struct stat) == 144, "stat size");
_Static_assert(offsetof(struct stat, st_nlink) == 16, "st_nlink offset");
_Static_assert(sizeof(((struct stat *)0)->st_nlink) == 8, "st_nlink width");
_Static_assert(offsetof(struct stat, st_mode) == 24, "st_mode offset");
_Static_assert(offsetof(struct stat, st_blksize) == 56, "st_blksize offset");
_Static_assert(sizeof(((struct stat *)0)->st_blksize) == 8, "st_blksize width");
_Static_assert(offsetof(struct stat, st_blocks) == 64, "st_blocks offset");
_Static_assert(sizeof(struct stat64) == 144, "stat64 size");
#else
_Static_assert(sizeof(struct stat) == 128, "stat size");
_Static_assert(offsetof(struct stat, st_mode) == 16, "st_mode offset");
_Static_assert(offsetof(struct stat, st_nlink) == 20, "st_nlink offset");
_Static_assert(sizeof(((struct stat *)0)->st_nlink) == 4, "st_nlink width");
_Static_assert(offsetof(struct stat, st_blksize) == 56, "st_blksize offset");
_Static_assert(sizeof(((struct stat *)0)->st_blksize) == 4, "st_blksize width");
_Static_assert(offsetof(struct stat, st_blocks) == 64, "st_blocks offset");
_Static_assert(sizeof(((struct stat *)0)->st_blocks) == 8, "st_blocks width");
_Static_assert(sizeof(struct stat64) == 128, "stat64 size");
#endif

_Static_assert(sizeof(struct dirent) == 280, "dirent size");
_Static_assert(sizeof(struct dirent64) == 280, "dirent64 size");
_Static_assert(sizeof(struct statvfs) == 112, "statvfs size");
_Static_assert(sizeof(blksize_t) == 8, "public blksize_t width");
_Static_assert(sizeof(blkcnt_t) == 8, "public blkcnt_t width");
_Static_assert((blksize_t)-1 > 0, "public blksize_t is unsigned");
_Static_assert(AT_FDCWD == -100, "AT_FDCWD");
_Static_assert(O_CREAT == 0100, "O_CREAT");
#if defined(__SLATE_ARCH_X86_64)
_Static_assert(O_DIRECTORY == 0200000, "O_DIRECTORY");
#else
_Static_assert(O_DIRECTORY == 040000, "O_DIRECTORY");
#endif
_Static_assert(F_DUPFD_CLOEXEC == 1030, "F_DUPFD_CLOEXEC");

int main(void) {
  struct stat    s;
  struct dirent  d;
  struct statvfs v;
  struct utimbuf u;
  (void)s;
  (void)d;
  (void)v;
  (void)u;
  return 0;
}
