#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

_Static_assert(sizeof(dev_t) == 4, "dev_t");
_Static_assert(sizeof(ino_t) == 8, "ino_t");
_Static_assert(sizeof(mode_t) == 2, "mode_t");
_Static_assert(sizeof(nlink_t) == 2, "nlink_t");
_Static_assert(sizeof(blksize_t) == 4, "blksize_t");
_Static_assert(sizeof(blkcnt_t) == 8, "blkcnt_t");
_Static_assert(sizeof(off_t) == 8, "off_t");

_Static_assert(sizeof(struct stat) == 144, "struct stat");
_Static_assert(offsetof(struct stat, st_dev) == 0, "stat st_dev");
_Static_assert(offsetof(struct stat, st_mode) == 4, "stat st_mode");
_Static_assert(offsetof(struct stat, st_nlink) == 6, "stat st_nlink");
_Static_assert(offsetof(struct stat, st_ino) == 8, "stat st_ino");
_Static_assert(offsetof(struct stat, st_atimespec) == 32, "stat st_atimespec");
_Static_assert(offsetof(struct stat, st_birthtimespec) == 80,
               "stat st_birthtimespec");
_Static_assert(offsetof(struct stat, st_size) == 96, "stat st_size");
_Static_assert(offsetof(struct stat, st_qspare) == 128, "stat st_qspare");

_Static_assert(sizeof(struct dirent) == 1048, "struct dirent");
_Static_assert(offsetof(struct dirent, d_ino) == 0, "dirent d_ino");
_Static_assert(offsetof(struct dirent, d_seekoff) == 8, "dirent d_seekoff");
_Static_assert(offsetof(struct dirent, d_reclen) == 16, "dirent d_reclen");
_Static_assert(offsetof(struct dirent, d_namlen) == 18, "dirent d_namlen");
_Static_assert(offsetof(struct dirent, d_type) == 20, "dirent d_type");
_Static_assert(offsetof(struct dirent, d_name) == 21, "dirent d_name");

_Static_assert(sizeof(struct statvfs) == 64, "struct statvfs");
_Static_assert(offsetof(struct statvfs, f_fsid) == 40, "statvfs f_fsid");

_Static_assert(O_RDONLY == 0x0000, "O_RDONLY");
_Static_assert(O_WRONLY == 0x0001, "O_WRONLY");
_Static_assert(O_NONBLOCK == 0x0004, "O_NONBLOCK");
_Static_assert(O_APPEND == 0x0008, "O_APPEND");
_Static_assert(O_CREAT == 0x0200, "O_CREAT");
_Static_assert(O_TRUNC == 0x0400, "O_TRUNC");
_Static_assert(O_EXCL == 0x0800, "O_EXCL");
_Static_assert(O_CLOEXEC == 0x01000000, "O_CLOEXEC");
_Static_assert(F_DUPFD_CLOEXEC == 67, "F_DUPFD_CLOEXEC");
_Static_assert(AT_FDCWD == -2, "AT_FDCWD");

_Static_assert(__builtin_types_compatible_p(__typeof__(&stat),
                                            int (*)(const char *__restrict,
                                                    struct stat *__restrict)),
               "stat signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&readdir),
                                            struct dirent *(*)(DIR *)),
               "readdir signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&open),
                                            int (*)(const char *, int, ...)),
               "open signature");

int is_directory(mode_t mode) { return S_ISDIR(mode); }

int stat_file(const char *path, struct stat *out) { return stat(path, out); }

struct dirent *next_entry(DIR *dir) { return readdir(dir); }

int open_read_only(const char *path) { return open(path, O_RDONLY); }

