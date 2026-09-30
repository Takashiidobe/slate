#include <sys/stat.h>

extern int slate_oracle_stat(const char *restrict, struct stat *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_stat), __typeof__(stat)),
    "sys/stat.h:stat declaration differs from oracle");

static __typeof__(stat) *const slate_reference_stat = &stat;

_Static_assert(__builtin_offsetof(struct stat, st_dev) == 0, "struct stat.st_dev offset differs from oracle");

typedef unsigned long long slate_oracle_struct_stat_st_dev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_dev), slate_oracle_struct_stat_st_dev), "struct stat.st_dev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, __st_dev_padding) == 8, "struct stat.__st_dev_padding offset differs from oracle");

typedef int slate_oracle_struct_stat___st_dev_padding;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->__st_dev_padding), slate_oracle_struct_stat___st_dev_padding), "struct stat.__st_dev_padding field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, __st_ino_truncated) == 12, "struct stat.__st_ino_truncated offset differs from oracle");

typedef long slate_oracle_struct_stat___st_ino_truncated;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->__st_ino_truncated), slate_oracle_struct_stat___st_ino_truncated), "struct stat.__st_ino_truncated field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_mode) == 16, "struct stat.st_mode offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mode), slate_oracle_struct_stat_st_mode), "struct stat.st_mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_nlink) == 20, "struct stat.st_nlink offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_nlink;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_nlink), slate_oracle_struct_stat_st_nlink), "struct stat.st_nlink field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_uid) == 24, "struct stat.st_uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_uid), slate_oracle_struct_stat_st_uid), "struct stat.st_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_gid) == 28, "struct stat.st_gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_gid), slate_oracle_struct_stat_st_gid), "struct stat.st_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_rdev) == 32, "struct stat.st_rdev offset differs from oracle");

typedef unsigned long long slate_oracle_struct_stat_st_rdev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_rdev), slate_oracle_struct_stat_st_rdev), "struct stat.st_rdev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, __st_rdev_padding) == 40, "struct stat.__st_rdev_padding offset differs from oracle");

typedef int slate_oracle_struct_stat___st_rdev_padding;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->__st_rdev_padding), slate_oracle_struct_stat___st_rdev_padding), "struct stat.__st_rdev_padding field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_size) == 44, "struct stat.st_size offset differs from oracle");

typedef long long slate_oracle_struct_stat_st_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_size), slate_oracle_struct_stat_st_size), "struct stat.st_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_blksize) == 52, "struct stat.st_blksize offset differs from oracle");

typedef long slate_oracle_struct_stat_st_blksize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_blksize), slate_oracle_struct_stat_st_blksize), "struct stat.st_blksize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_blocks) == 56, "struct stat.st_blocks offset differs from oracle");

typedef long long slate_oracle_struct_stat_st_blocks;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_blocks), slate_oracle_struct_stat_st_blocks), "struct stat.st_blocks field type differs from oracle");

typedef unsigned long long slate_oracle_struct_stat_st_ino;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ino), slate_oracle_struct_stat_st_ino), "struct stat.st_ino field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_atim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_atim), slate_oracle_struct_stat_st_atim), "struct stat.st_atim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_mtim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mtim), slate_oracle_struct_stat_st_mtim), "struct stat.st_mtim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_ctim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ctim), slate_oracle_struct_stat_st_ctim), "struct stat.st_ctim field type differs from oracle");

#ifndef STATX_ALL
#error "sys/stat.h:STATX_ALL macro is missing from libc-shim"
#endif

#ifndef STATX_ATIME
#error "sys/stat.h:STATX_ATIME macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_APPEND
#error "sys/stat.h:STATX_ATTR_APPEND macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_AUTOMOUNT
#error "sys/stat.h:STATX_ATTR_AUTOMOUNT macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_COMPRESSED
#error "sys/stat.h:STATX_ATTR_COMPRESSED macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_DAX
#error "sys/stat.h:STATX_ATTR_DAX macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_ENCRYPTED
#error "sys/stat.h:STATX_ATTR_ENCRYPTED macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_IMMUTABLE
#error "sys/stat.h:STATX_ATTR_IMMUTABLE macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_MOUNT_ROOT
#error "sys/stat.h:STATX_ATTR_MOUNT_ROOT macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_NODUMP
#error "sys/stat.h:STATX_ATTR_NODUMP macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_VERITY
#error "sys/stat.h:STATX_ATTR_VERITY macro is missing from libc-shim"
#endif

#ifndef STATX_ATTR_WRITE_ATOMIC
#error "sys/stat.h:STATX_ATTR_WRITE_ATOMIC macro is missing from libc-shim"
#endif

#ifndef STATX_BASIC_STATS
#error "sys/stat.h:STATX_BASIC_STATS macro is missing from libc-shim"
#endif

#ifndef STATX_BLOCKS
#error "sys/stat.h:STATX_BLOCKS macro is missing from libc-shim"
#endif

#ifndef STATX_BTIME
#error "sys/stat.h:STATX_BTIME macro is missing from libc-shim"
#endif

#ifndef STATX_CTIME
#error "sys/stat.h:STATX_CTIME macro is missing from libc-shim"
#endif

#ifndef STATX_DIOALIGN
#error "sys/stat.h:STATX_DIOALIGN macro is missing from libc-shim"
#endif

#ifndef STATX_GID
#error "sys/stat.h:STATX_GID macro is missing from libc-shim"
#endif

#ifndef STATX_INO
#error "sys/stat.h:STATX_INO macro is missing from libc-shim"
#endif

#ifndef STATX_MNT_ID
#error "sys/stat.h:STATX_MNT_ID macro is missing from libc-shim"
#endif

#ifndef STATX_MNT_ID_UNIQUE
#error "sys/stat.h:STATX_MNT_ID_UNIQUE macro is missing from libc-shim"
#endif

#ifndef STATX_MODE
#error "sys/stat.h:STATX_MODE macro is missing from libc-shim"
#endif

#ifndef STATX_MTIME
#error "sys/stat.h:STATX_MTIME macro is missing from libc-shim"
#endif

#ifndef STATX_NLINK
#error "sys/stat.h:STATX_NLINK macro is missing from libc-shim"
#endif

#ifndef STATX_SIZE
#error "sys/stat.h:STATX_SIZE macro is missing from libc-shim"
#endif

#ifndef STATX_SUBVOL
#error "sys/stat.h:STATX_SUBVOL macro is missing from libc-shim"
#endif

#ifndef STATX_TYPE
#error "sys/stat.h:STATX_TYPE macro is missing from libc-shim"
#endif

#ifndef STATX_UID
#error "sys/stat.h:STATX_UID macro is missing from libc-shim"
#endif

#ifndef STATX_WRITE_ATOMIC
#error "sys/stat.h:STATX_WRITE_ATOMIC macro is missing from libc-shim"
#endif

#ifndef S_IEXEC
#error "sys/stat.h:S_IEXEC macro is missing from libc-shim"
#endif

#ifndef S_IFBLK
#error "sys/stat.h:S_IFBLK macro is missing from libc-shim"
#endif

#ifndef S_IFCHR
#error "sys/stat.h:S_IFCHR macro is missing from libc-shim"
#endif

#ifndef S_IFDIR
#error "sys/stat.h:S_IFDIR macro is missing from libc-shim"
#endif

#ifndef S_IFIFO
#error "sys/stat.h:S_IFIFO macro is missing from libc-shim"
#endif

#ifndef S_IFLNK
#error "sys/stat.h:S_IFLNK macro is missing from libc-shim"
#endif

#ifndef S_IFMT
#error "sys/stat.h:S_IFMT macro is missing from libc-shim"
#endif

#ifndef S_IFREG
#error "sys/stat.h:S_IFREG macro is missing from libc-shim"
#endif

#ifndef S_IFSOCK
#error "sys/stat.h:S_IFSOCK macro is missing from libc-shim"
#endif

#ifndef S_IREAD
#error "sys/stat.h:S_IREAD macro is missing from libc-shim"
#endif

#ifndef S_IRGRP
#error "sys/stat.h:S_IRGRP macro is missing from libc-shim"
#endif

#ifndef S_IROTH
#error "sys/stat.h:S_IROTH macro is missing from libc-shim"
#endif

#ifndef S_IRUSR
#error "sys/stat.h:S_IRUSR macro is missing from libc-shim"
#endif

#ifndef S_IRWXG
#error "sys/stat.h:S_IRWXG macro is missing from libc-shim"
#endif

#ifndef S_IRWXO
#error "sys/stat.h:S_IRWXO macro is missing from libc-shim"
#endif

#ifndef S_IRWXU
#error "sys/stat.h:S_IRWXU macro is missing from libc-shim"
#endif

#ifndef S_ISBLK
#error "sys/stat.h:S_ISBLK macro is missing from libc-shim"
#endif

#ifndef S_ISCHR
#error "sys/stat.h:S_ISCHR macro is missing from libc-shim"
#endif

#ifndef S_ISDIR
#error "sys/stat.h:S_ISDIR macro is missing from libc-shim"
#endif

#ifndef S_ISFIFO
#error "sys/stat.h:S_ISFIFO macro is missing from libc-shim"
#endif

#ifndef S_ISGID
#error "sys/stat.h:S_ISGID macro is missing from libc-shim"
#endif

#ifndef S_ISLNK
#error "sys/stat.h:S_ISLNK macro is missing from libc-shim"
#endif

#ifndef S_ISREG
#error "sys/stat.h:S_ISREG macro is missing from libc-shim"
#endif

#ifndef S_ISSOCK
#error "sys/stat.h:S_ISSOCK macro is missing from libc-shim"
#endif

#ifndef S_ISUID
#error "sys/stat.h:S_ISUID macro is missing from libc-shim"
#endif

#ifndef S_ISVTX
#error "sys/stat.h:S_ISVTX macro is missing from libc-shim"
#endif

#ifndef S_IWGRP
#error "sys/stat.h:S_IWGRP macro is missing from libc-shim"
#endif

#ifndef S_IWOTH
#error "sys/stat.h:S_IWOTH macro is missing from libc-shim"
#endif

#ifndef S_IWRITE
#error "sys/stat.h:S_IWRITE macro is missing from libc-shim"
#endif

#ifndef S_IWUSR
#error "sys/stat.h:S_IWUSR macro is missing from libc-shim"
#endif

#ifndef S_IXGRP
#error "sys/stat.h:S_IXGRP macro is missing from libc-shim"
#endif

#ifndef S_IXOTH
#error "sys/stat.h:S_IXOTH macro is missing from libc-shim"
#endif

#ifndef S_IXUSR
#error "sys/stat.h:S_IXUSR macro is missing from libc-shim"
#endif

#ifndef S_TYPEISMQ
#error "sys/stat.h:S_TYPEISMQ macro is missing from libc-shim"
#endif

#ifndef S_TYPEISSEM
#error "sys/stat.h:S_TYPEISSEM macro is missing from libc-shim"
#endif

#ifndef S_TYPEISSHM
#error "sys/stat.h:S_TYPEISSHM macro is missing from libc-shim"
#endif

#ifndef S_TYPEISTMO
#error "sys/stat.h:S_TYPEISTMO macro is missing from libc-shim"
#endif

#ifndef UTIME_NOW
#error "sys/stat.h:UTIME_NOW macro is missing from libc-shim"
#endif

#ifndef UTIME_OMIT
#error "sys/stat.h:UTIME_OMIT macro is missing from libc-shim"
#endif

#ifndef st_atime
#error "sys/stat.h:st_atime macro is missing from libc-shim"
#endif

#ifndef st_ctime
#error "sys/stat.h:st_ctime macro is missing from libc-shim"
#endif

#ifndef st_mtime
#error "sys/stat.h:st_mtime macro is missing from libc-shim"
#endif

int main(void) { return 0; }
