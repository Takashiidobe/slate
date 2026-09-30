#include <fcntl.h>

extern int slate_oracle_fcntl(int, int, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcntl), __typeof__(fcntl)),
    "fcntl.h:fcntl declaration differs from oracle");

static __typeof__(fcntl) *const slate_reference_fcntl = &fcntl;

typedef unsigned int slate_oracle_typedef_mode_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mode_t, mode_t), "typedef mode_t differs from oracle");

_Static_assert(sizeof(struct flock) == 16, "struct flock size differs from oracle");

_Static_assert(_Alignof(struct flock) == 4, "struct flock alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_type) == 0, "struct flock.l_type offset differs from oracle");

typedef short slate_oracle_struct_flock_l_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_type), slate_oracle_struct_flock_l_type), "struct flock.l_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_whence) == 2, "struct flock.l_whence offset differs from oracle");

typedef short slate_oracle_struct_flock_l_whence;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_whence), slate_oracle_struct_flock_l_whence), "struct flock.l_whence field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_start) == 4, "struct flock.l_start offset differs from oracle");

typedef long slate_oracle_struct_flock_l_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_start), slate_oracle_struct_flock_l_start), "struct flock.l_start field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_len) == 8, "struct flock.l_len offset differs from oracle");

typedef long slate_oracle_struct_flock_l_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_len), slate_oracle_struct_flock_l_len), "struct flock.l_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_pid) == 12, "struct flock.l_pid offset differs from oracle");

typedef int slate_oracle_struct_flock_l_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_pid), slate_oracle_struct_flock_l_pid), "struct flock.l_pid field type differs from oracle");

#ifndef AT_EACCESS
#error "fcntl.h:AT_EACCESS macro is missing from libc-shim"
#endif

#ifndef AT_EMPTY_PATH
#error "fcntl.h:AT_EMPTY_PATH macro is missing from libc-shim"
#endif

#ifndef AT_FDCWD
#error "fcntl.h:AT_FDCWD macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_FID
#error "fcntl.h:AT_HANDLE_FID macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_MNT_ID_UNIQUE
#error "fcntl.h:AT_HANDLE_MNT_ID_UNIQUE macro is missing from libc-shim"
#endif

#ifndef AT_NO_AUTOMOUNT
#error "fcntl.h:AT_NO_AUTOMOUNT macro is missing from libc-shim"
#endif

#ifndef AT_RECURSIVE
#error "fcntl.h:AT_RECURSIVE macro is missing from libc-shim"
#endif

#ifndef AT_REMOVEDIR
#error "fcntl.h:AT_REMOVEDIR macro is missing from libc-shim"
#endif

#ifndef AT_STATX_DONT_SYNC
#error "fcntl.h:AT_STATX_DONT_SYNC macro is missing from libc-shim"
#endif

#ifndef AT_STATX_FORCE_SYNC
#error "fcntl.h:AT_STATX_FORCE_SYNC macro is missing from libc-shim"
#endif

#ifndef AT_STATX_SYNC_AS_STAT
#error "fcntl.h:AT_STATX_SYNC_AS_STAT macro is missing from libc-shim"
#endif

#ifndef AT_STATX_SYNC_TYPE
#error "fcntl.h:AT_STATX_SYNC_TYPE macro is missing from libc-shim"
#endif

#ifndef AT_SYMLINK_FOLLOW
#error "fcntl.h:AT_SYMLINK_FOLLOW macro is missing from libc-shim"
#endif

#ifndef AT_SYMLINK_NOFOLLOW
#error "fcntl.h:AT_SYMLINK_NOFOLLOW macro is missing from libc-shim"
#endif

#ifndef DN_ACCESS
#error "fcntl.h:DN_ACCESS macro is missing from libc-shim"
#endif

#ifndef DN_ATTRIB
#error "fcntl.h:DN_ATTRIB macro is missing from libc-shim"
#endif

#ifndef DN_CREATE
#error "fcntl.h:DN_CREATE macro is missing from libc-shim"
#endif

#ifndef DN_DELETE
#error "fcntl.h:DN_DELETE macro is missing from libc-shim"
#endif

#ifndef DN_MODIFY
#error "fcntl.h:DN_MODIFY macro is missing from libc-shim"
#endif

#ifndef DN_MULTISHOT
#error "fcntl.h:DN_MULTISHOT macro is missing from libc-shim"
#endif

#ifndef DN_RENAME
#error "fcntl.h:DN_RENAME macro is missing from libc-shim"
#endif

#ifndef FAPPEND
#error "fcntl.h:FAPPEND macro is missing from libc-shim"
#endif

#ifndef FASYNC
#error "fcntl.h:FASYNC macro is missing from libc-shim"
#endif

#ifndef FD_CLOEXEC
#error "fcntl.h:FD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef FFSYNC
#error "fcntl.h:FFSYNC macro is missing from libc-shim"
#endif

#ifndef FNDELAY
#error "fcntl.h:FNDELAY macro is missing from libc-shim"
#endif

#ifndef FNONBLOCK
#error "fcntl.h:FNONBLOCK macro is missing from libc-shim"
#endif

#ifndef F_ADD_SEALS
#error "fcntl.h:F_ADD_SEALS macro is missing from libc-shim"
#endif

#ifndef F_CREATED_QUERY
#error "fcntl.h:F_CREATED_QUERY macro is missing from libc-shim"
#endif

#ifndef F_DUPFD
#error "fcntl.h:F_DUPFD macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_CLOEXEC
#error "fcntl.h:F_DUPFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_QUERY
#error "fcntl.h:F_DUPFD_QUERY macro is missing from libc-shim"
#endif

#ifndef F_EXLCK
#error "fcntl.h:F_EXLCK macro is missing from libc-shim"
#endif

#ifndef F_GETFD
#error "fcntl.h:F_GETFD macro is missing from libc-shim"
#endif

#ifndef F_GETFL
#error "fcntl.h:F_GETFL macro is missing from libc-shim"
#endif

#ifndef F_GETLEASE
#error "fcntl.h:F_GETLEASE macro is missing from libc-shim"
#endif

#ifndef F_GETLK
#error "fcntl.h:F_GETLK macro is missing from libc-shim"
#endif

#ifndef F_GETLK64
#error "fcntl.h:F_GETLK64 macro is missing from libc-shim"
#endif

#ifndef F_GETOWN
#error "fcntl.h:F_GETOWN macro is missing from libc-shim"
#endif

#ifndef F_GETOWN_EX
#error "fcntl.h:F_GETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_GETPIPE_SZ
#error "fcntl.h:F_GETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_GETSIG
#error "fcntl.h:F_GETSIG macro is missing from libc-shim"
#endif

#ifndef F_GET_FILE_RW_HINT
#error "fcntl.h:F_GET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_RW_HINT
#error "fcntl.h:F_GET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_SEALS
#error "fcntl.h:F_GET_SEALS macro is missing from libc-shim"
#endif

#ifndef F_LOCK
#error "fcntl.h:F_LOCK macro is missing from libc-shim"
#endif

#ifndef F_NOTIFY
#error "fcntl.h:F_NOTIFY macro is missing from libc-shim"
#endif

#ifndef F_OFD_GETLK
#error "fcntl.h:F_OFD_GETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLK
#error "fcntl.h:F_OFD_SETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLKW
#error "fcntl.h:F_OFD_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_OK
#error "fcntl.h:F_OK macro is missing from libc-shim"
#endif

#ifndef F_RDLCK
#error "fcntl.h:F_RDLCK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_EXEC
#error "fcntl.h:F_SEAL_EXEC macro is missing from libc-shim"
#endif

#ifndef F_SEAL_FUTURE_WRITE
#error "fcntl.h:F_SEAL_FUTURE_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SEAL_GROW
#error "fcntl.h:F_SEAL_GROW macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SEAL
#error "fcntl.h:F_SEAL_SEAL macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SHRINK
#error "fcntl.h:F_SEAL_SHRINK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_WRITE
#error "fcntl.h:F_SEAL_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SETFD
#error "fcntl.h:F_SETFD macro is missing from libc-shim"
#endif

#ifndef F_SETFL
#error "fcntl.h:F_SETFL macro is missing from libc-shim"
#endif

#ifndef F_SETLEASE
#error "fcntl.h:F_SETLEASE macro is missing from libc-shim"
#endif

#ifndef F_SETLK
#error "fcntl.h:F_SETLK macro is missing from libc-shim"
#endif

#ifndef F_SETLK64
#error "fcntl.h:F_SETLK64 macro is missing from libc-shim"
#endif

#ifndef F_SETLKW
#error "fcntl.h:F_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_SETLKW64
#error "fcntl.h:F_SETLKW64 macro is missing from libc-shim"
#endif

#ifndef F_SETOWN
#error "fcntl.h:F_SETOWN macro is missing from libc-shim"
#endif

#ifndef F_SETOWN_EX
#error "fcntl.h:F_SETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_SETPIPE_SZ
#error "fcntl.h:F_SETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_SETSIG
#error "fcntl.h:F_SETSIG macro is missing from libc-shim"
#endif

#ifndef F_SET_FILE_RW_HINT
#error "fcntl.h:F_SET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SET_RW_HINT
#error "fcntl.h:F_SET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SHLCK
#error "fcntl.h:F_SHLCK macro is missing from libc-shim"
#endif

#ifndef F_TEST
#error "fcntl.h:F_TEST macro is missing from libc-shim"
#endif

#ifndef F_TLOCK
#error "fcntl.h:F_TLOCK macro is missing from libc-shim"
#endif

#ifndef F_ULOCK
#error "fcntl.h:F_ULOCK macro is missing from libc-shim"
#endif

#ifndef F_UNLCK
#error "fcntl.h:F_UNLCK macro is missing from libc-shim"
#endif

#ifndef F_WRLCK
#error "fcntl.h:F_WRLCK macro is missing from libc-shim"
#endif

#ifndef LOCK_EX
#error "fcntl.h:LOCK_EX macro is missing from libc-shim"
#endif

#ifndef LOCK_MAND
#error "fcntl.h:LOCK_MAND macro is missing from libc-shim"
#endif

#ifndef LOCK_NB
#error "fcntl.h:LOCK_NB macro is missing from libc-shim"
#endif

#ifndef LOCK_READ
#error "fcntl.h:LOCK_READ macro is missing from libc-shim"
#endif

#ifndef LOCK_RW
#error "fcntl.h:LOCK_RW macro is missing from libc-shim"
#endif

#ifndef LOCK_SH
#error "fcntl.h:LOCK_SH macro is missing from libc-shim"
#endif

#ifndef LOCK_UN
#error "fcntl.h:LOCK_UN macro is missing from libc-shim"
#endif

#ifndef LOCK_WRITE
#error "fcntl.h:LOCK_WRITE macro is missing from libc-shim"
#endif

#ifndef MAX_HANDLE_SZ
#error "fcntl.h:MAX_HANDLE_SZ macro is missing from libc-shim"
#endif

#ifndef O_ACCMODE
#error "fcntl.h:O_ACCMODE macro is missing from libc-shim"
#endif

#ifndef O_APPEND
#error "fcntl.h:O_APPEND macro is missing from libc-shim"
#endif

#ifndef O_ASYNC
#error "fcntl.h:O_ASYNC macro is missing from libc-shim"
#endif

#ifndef O_CLOEXEC
#error "fcntl.h:O_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef O_CREAT
#error "fcntl.h:O_CREAT macro is missing from libc-shim"
#endif

#ifndef O_DIRECT
#error "fcntl.h:O_DIRECT macro is missing from libc-shim"
#endif

#ifndef O_DIRECTORY
#error "fcntl.h:O_DIRECTORY macro is missing from libc-shim"
#endif

#ifndef O_DSYNC
#error "fcntl.h:O_DSYNC macro is missing from libc-shim"
#endif

#ifndef O_EXCL
#error "fcntl.h:O_EXCL macro is missing from libc-shim"
#endif

#ifndef O_FSYNC
#error "fcntl.h:O_FSYNC macro is missing from libc-shim"
#endif

#ifndef O_LARGEFILE
#error "fcntl.h:O_LARGEFILE macro is missing from libc-shim"
#endif

#ifndef O_NDELAY
#error "fcntl.h:O_NDELAY macro is missing from libc-shim"
#endif

#ifndef O_NOATIME
#error "fcntl.h:O_NOATIME macro is missing from libc-shim"
#endif

#ifndef O_NOCTTY
#error "fcntl.h:O_NOCTTY macro is missing from libc-shim"
#endif

#ifndef O_NOFOLLOW
#error "fcntl.h:O_NOFOLLOW macro is missing from libc-shim"
#endif

#ifndef O_NONBLOCK
#error "fcntl.h:O_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef O_PATH
#error "fcntl.h:O_PATH macro is missing from libc-shim"
#endif

#ifndef O_RDONLY
#error "fcntl.h:O_RDONLY macro is missing from libc-shim"
#endif

#ifndef O_RDWR
#error "fcntl.h:O_RDWR macro is missing from libc-shim"
#endif

#ifndef O_RSYNC
#error "fcntl.h:O_RSYNC macro is missing from libc-shim"
#endif

#ifndef O_SYNC
#error "fcntl.h:O_SYNC macro is missing from libc-shim"
#endif

#ifndef O_TMPFILE
#error "fcntl.h:O_TMPFILE macro is missing from libc-shim"
#endif

#ifndef O_TRUNC
#error "fcntl.h:O_TRUNC macro is missing from libc-shim"
#endif

#ifndef O_WRONLY
#error "fcntl.h:O_WRONLY macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_DONTNEED
#error "fcntl.h:POSIX_FADV_DONTNEED macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NOREUSE
#error "fcntl.h:POSIX_FADV_NOREUSE macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NORMAL
#error "fcntl.h:POSIX_FADV_NORMAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_RANDOM
#error "fcntl.h:POSIX_FADV_RANDOM macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_SEQUENTIAL
#error "fcntl.h:POSIX_FADV_SEQUENTIAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_WILLNEED
#error "fcntl.h:POSIX_FADV_WILLNEED macro is missing from libc-shim"
#endif

#ifndef RWF_WRITE_LIFE_NOT_SET
#error "fcntl.h:RWF_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_EXTREME
#error "fcntl.h:RWH_WRITE_LIFE_EXTREME macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_LONG
#error "fcntl.h:RWH_WRITE_LIFE_LONG macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_MEDIUM
#error "fcntl.h:RWH_WRITE_LIFE_MEDIUM macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NONE
#error "fcntl.h:RWH_WRITE_LIFE_NONE macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NOT_SET
#error "fcntl.h:RWH_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_SHORT
#error "fcntl.h:RWH_WRITE_LIFE_SHORT macro is missing from libc-shim"
#endif

#ifndef R_OK
#error "fcntl.h:R_OK macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "fcntl.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "fcntl.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "fcntl.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_GIFT
#error "fcntl.h:SPLICE_F_GIFT macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MORE
#error "fcntl.h:SPLICE_F_MORE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MOVE
#error "fcntl.h:SPLICE_F_MOVE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_NONBLOCK
#error "fcntl.h:SPLICE_F_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_AFTER
#error "fcntl.h:SYNC_FILE_RANGE_WAIT_AFTER macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_BEFORE
#error "fcntl.h:SYNC_FILE_RANGE_WAIT_BEFORE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE
#error "fcntl.h:SYNC_FILE_RANGE_WRITE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE_AND_WAIT
#error "fcntl.h:SYNC_FILE_RANGE_WRITE_AND_WAIT macro is missing from libc-shim"
#endif

#ifndef S_IFBLK
#error "fcntl.h:S_IFBLK macro is missing from libc-shim"
#endif

#ifndef S_IFCHR
#error "fcntl.h:S_IFCHR macro is missing from libc-shim"
#endif

#ifndef S_IFDIR
#error "fcntl.h:S_IFDIR macro is missing from libc-shim"
#endif

#ifndef S_IFIFO
#error "fcntl.h:S_IFIFO macro is missing from libc-shim"
#endif

#ifndef S_IFLNK
#error "fcntl.h:S_IFLNK macro is missing from libc-shim"
#endif

#ifndef S_IFMT
#error "fcntl.h:S_IFMT macro is missing from libc-shim"
#endif

#ifndef S_IFREG
#error "fcntl.h:S_IFREG macro is missing from libc-shim"
#endif

#ifndef S_IFSOCK
#error "fcntl.h:S_IFSOCK macro is missing from libc-shim"
#endif

#ifndef S_IRGRP
#error "fcntl.h:S_IRGRP macro is missing from libc-shim"
#endif

#ifndef S_IROTH
#error "fcntl.h:S_IROTH macro is missing from libc-shim"
#endif

#ifndef S_IRUSR
#error "fcntl.h:S_IRUSR macro is missing from libc-shim"
#endif

#ifndef S_IRWXG
#error "fcntl.h:S_IRWXG macro is missing from libc-shim"
#endif

#ifndef S_IRWXO
#error "fcntl.h:S_IRWXO macro is missing from libc-shim"
#endif

#ifndef S_IRWXU
#error "fcntl.h:S_IRWXU macro is missing from libc-shim"
#endif

#ifndef S_ISGID
#error "fcntl.h:S_ISGID macro is missing from libc-shim"
#endif

#ifndef S_ISUID
#error "fcntl.h:S_ISUID macro is missing from libc-shim"
#endif

#ifndef S_ISVTX
#error "fcntl.h:S_ISVTX macro is missing from libc-shim"
#endif

#ifndef S_IWGRP
#error "fcntl.h:S_IWGRP macro is missing from libc-shim"
#endif

#ifndef S_IWOTH
#error "fcntl.h:S_IWOTH macro is missing from libc-shim"
#endif

#ifndef S_IWUSR
#error "fcntl.h:S_IWUSR macro is missing from libc-shim"
#endif

#ifndef S_IXGRP
#error "fcntl.h:S_IXGRP macro is missing from libc-shim"
#endif

#ifndef S_IXOTH
#error "fcntl.h:S_IXOTH macro is missing from libc-shim"
#endif

#ifndef S_IXUSR
#error "fcntl.h:S_IXUSR macro is missing from libc-shim"
#endif

#ifndef W_OK
#error "fcntl.h:W_OK macro is missing from libc-shim"
#endif

#ifndef X_OK
#error "fcntl.h:X_OK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
