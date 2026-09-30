#include <sys/fcntl.h>

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

#ifndef AT_HANDLE_FID
#error "sys/fcntl.h:AT_HANDLE_FID macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_MNT_ID_UNIQUE
#error "sys/fcntl.h:AT_HANDLE_MNT_ID_UNIQUE macro is missing from libc-shim"
#endif

#ifndef DN_ACCESS
#error "sys/fcntl.h:DN_ACCESS macro is missing from libc-shim"
#endif

#ifndef DN_ATTRIB
#error "sys/fcntl.h:DN_ATTRIB macro is missing from libc-shim"
#endif

#ifndef DN_CREATE
#error "sys/fcntl.h:DN_CREATE macro is missing from libc-shim"
#endif

#ifndef DN_DELETE
#error "sys/fcntl.h:DN_DELETE macro is missing from libc-shim"
#endif

#ifndef DN_MODIFY
#error "sys/fcntl.h:DN_MODIFY macro is missing from libc-shim"
#endif

#ifndef DN_MULTISHOT
#error "sys/fcntl.h:DN_MULTISHOT macro is missing from libc-shim"
#endif

#ifndef DN_RENAME
#error "sys/fcntl.h:DN_RENAME macro is missing from libc-shim"
#endif

#ifndef FAPPEND
#error "sys/fcntl.h:FAPPEND macro is missing from libc-shim"
#endif

#ifndef FASYNC
#error "sys/fcntl.h:FASYNC macro is missing from libc-shim"
#endif

#ifndef FD_CLOEXEC
#error "sys/fcntl.h:FD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef FFSYNC
#error "sys/fcntl.h:FFSYNC macro is missing from libc-shim"
#endif

#ifndef FNDELAY
#error "sys/fcntl.h:FNDELAY macro is missing from libc-shim"
#endif

#ifndef FNONBLOCK
#error "sys/fcntl.h:FNONBLOCK macro is missing from libc-shim"
#endif

#ifndef F_ADD_SEALS
#error "sys/fcntl.h:F_ADD_SEALS macro is missing from libc-shim"
#endif

#ifndef F_CREATED_QUERY
#error "sys/fcntl.h:F_CREATED_QUERY macro is missing from libc-shim"
#endif

#ifndef F_DUPFD
#error "sys/fcntl.h:F_DUPFD macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_CLOEXEC
#error "sys/fcntl.h:F_DUPFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_QUERY
#error "sys/fcntl.h:F_DUPFD_QUERY macro is missing from libc-shim"
#endif

#ifndef F_EXLCK
#error "sys/fcntl.h:F_EXLCK macro is missing from libc-shim"
#endif

#ifndef F_GETFD
#error "sys/fcntl.h:F_GETFD macro is missing from libc-shim"
#endif

#ifndef F_GETFL
#error "sys/fcntl.h:F_GETFL macro is missing from libc-shim"
#endif

#ifndef F_GETLEASE
#error "sys/fcntl.h:F_GETLEASE macro is missing from libc-shim"
#endif

#ifndef F_GETLK
#error "sys/fcntl.h:F_GETLK macro is missing from libc-shim"
#endif

#ifndef F_GETLK64
#error "sys/fcntl.h:F_GETLK64 macro is missing from libc-shim"
#endif

#ifndef F_GETOWN
#error "sys/fcntl.h:F_GETOWN macro is missing from libc-shim"
#endif

#ifndef F_GETOWN_EX
#error "sys/fcntl.h:F_GETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_GETPIPE_SZ
#error "sys/fcntl.h:F_GETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_GETSIG
#error "sys/fcntl.h:F_GETSIG macro is missing from libc-shim"
#endif

#ifndef F_GET_FILE_RW_HINT
#error "sys/fcntl.h:F_GET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_RW_HINT
#error "sys/fcntl.h:F_GET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_SEALS
#error "sys/fcntl.h:F_GET_SEALS macro is missing from libc-shim"
#endif

#ifndef F_NOTIFY
#error "sys/fcntl.h:F_NOTIFY macro is missing from libc-shim"
#endif

#ifndef F_OFD_GETLK
#error "sys/fcntl.h:F_OFD_GETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLK
#error "sys/fcntl.h:F_OFD_SETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLKW
#error "sys/fcntl.h:F_OFD_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_RDLCK
#error "sys/fcntl.h:F_RDLCK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_EXEC
#error "sys/fcntl.h:F_SEAL_EXEC macro is missing from libc-shim"
#endif

#ifndef F_SEAL_FUTURE_WRITE
#error "sys/fcntl.h:F_SEAL_FUTURE_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SEAL_GROW
#error "sys/fcntl.h:F_SEAL_GROW macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SEAL
#error "sys/fcntl.h:F_SEAL_SEAL macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SHRINK
#error "sys/fcntl.h:F_SEAL_SHRINK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_WRITE
#error "sys/fcntl.h:F_SEAL_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SETFD
#error "sys/fcntl.h:F_SETFD macro is missing from libc-shim"
#endif

#ifndef F_SETFL
#error "sys/fcntl.h:F_SETFL macro is missing from libc-shim"
#endif

#ifndef F_SETLEASE
#error "sys/fcntl.h:F_SETLEASE macro is missing from libc-shim"
#endif

#ifndef F_SETLK
#error "sys/fcntl.h:F_SETLK macro is missing from libc-shim"
#endif

#ifndef F_SETLK64
#error "sys/fcntl.h:F_SETLK64 macro is missing from libc-shim"
#endif

#ifndef F_SETLKW
#error "sys/fcntl.h:F_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_SETLKW64
#error "sys/fcntl.h:F_SETLKW64 macro is missing from libc-shim"
#endif

#ifndef F_SETOWN
#error "sys/fcntl.h:F_SETOWN macro is missing from libc-shim"
#endif

#ifndef F_SETOWN_EX
#error "sys/fcntl.h:F_SETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_SETPIPE_SZ
#error "sys/fcntl.h:F_SETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_SETSIG
#error "sys/fcntl.h:F_SETSIG macro is missing from libc-shim"
#endif

#ifndef F_SET_FILE_RW_HINT
#error "sys/fcntl.h:F_SET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SET_RW_HINT
#error "sys/fcntl.h:F_SET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SHLCK
#error "sys/fcntl.h:F_SHLCK macro is missing from libc-shim"
#endif

#ifndef F_UNLCK
#error "sys/fcntl.h:F_UNLCK macro is missing from libc-shim"
#endif

#ifndef F_WRLCK
#error "sys/fcntl.h:F_WRLCK macro is missing from libc-shim"
#endif

#ifndef LOCK_EX
#error "sys/fcntl.h:LOCK_EX macro is missing from libc-shim"
#endif

#ifndef LOCK_MAND
#error "sys/fcntl.h:LOCK_MAND macro is missing from libc-shim"
#endif

#ifndef LOCK_NB
#error "sys/fcntl.h:LOCK_NB macro is missing from libc-shim"
#endif

#ifndef LOCK_READ
#error "sys/fcntl.h:LOCK_READ macro is missing from libc-shim"
#endif

#ifndef LOCK_RW
#error "sys/fcntl.h:LOCK_RW macro is missing from libc-shim"
#endif

#ifndef LOCK_SH
#error "sys/fcntl.h:LOCK_SH macro is missing from libc-shim"
#endif

#ifndef LOCK_UN
#error "sys/fcntl.h:LOCK_UN macro is missing from libc-shim"
#endif

#ifndef LOCK_WRITE
#error "sys/fcntl.h:LOCK_WRITE macro is missing from libc-shim"
#endif

#ifndef MAX_HANDLE_SZ
#error "sys/fcntl.h:MAX_HANDLE_SZ macro is missing from libc-shim"
#endif

#ifndef O_ACCMODE
#error "sys/fcntl.h:O_ACCMODE macro is missing from libc-shim"
#endif

#ifndef O_APPEND
#error "sys/fcntl.h:O_APPEND macro is missing from libc-shim"
#endif

#ifndef O_ASYNC
#error "sys/fcntl.h:O_ASYNC macro is missing from libc-shim"
#endif

#ifndef O_CLOEXEC
#error "sys/fcntl.h:O_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef O_CREAT
#error "sys/fcntl.h:O_CREAT macro is missing from libc-shim"
#endif

#ifndef O_DIRECT
#error "sys/fcntl.h:O_DIRECT macro is missing from libc-shim"
#endif

#ifndef O_DIRECTORY
#error "sys/fcntl.h:O_DIRECTORY macro is missing from libc-shim"
#endif

#ifndef O_DSYNC
#error "sys/fcntl.h:O_DSYNC macro is missing from libc-shim"
#endif

#ifndef O_EXCL
#error "sys/fcntl.h:O_EXCL macro is missing from libc-shim"
#endif

#ifndef O_FSYNC
#error "sys/fcntl.h:O_FSYNC macro is missing from libc-shim"
#endif

#ifndef O_LARGEFILE
#error "sys/fcntl.h:O_LARGEFILE macro is missing from libc-shim"
#endif

#ifndef O_NDELAY
#error "sys/fcntl.h:O_NDELAY macro is missing from libc-shim"
#endif

#ifndef O_NOATIME
#error "sys/fcntl.h:O_NOATIME macro is missing from libc-shim"
#endif

#ifndef O_NOCTTY
#error "sys/fcntl.h:O_NOCTTY macro is missing from libc-shim"
#endif

#ifndef O_NOFOLLOW
#error "sys/fcntl.h:O_NOFOLLOW macro is missing from libc-shim"
#endif

#ifndef O_NONBLOCK
#error "sys/fcntl.h:O_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef O_PATH
#error "sys/fcntl.h:O_PATH macro is missing from libc-shim"
#endif

#ifndef O_RDONLY
#error "sys/fcntl.h:O_RDONLY macro is missing from libc-shim"
#endif

#ifndef O_RDWR
#error "sys/fcntl.h:O_RDWR macro is missing from libc-shim"
#endif

#ifndef O_RSYNC
#error "sys/fcntl.h:O_RSYNC macro is missing from libc-shim"
#endif

#ifndef O_SYNC
#error "sys/fcntl.h:O_SYNC macro is missing from libc-shim"
#endif

#ifndef O_TMPFILE
#error "sys/fcntl.h:O_TMPFILE macro is missing from libc-shim"
#endif

#ifndef O_TRUNC
#error "sys/fcntl.h:O_TRUNC macro is missing from libc-shim"
#endif

#ifndef O_WRONLY
#error "sys/fcntl.h:O_WRONLY macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_DONTNEED
#error "sys/fcntl.h:POSIX_FADV_DONTNEED macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NOREUSE
#error "sys/fcntl.h:POSIX_FADV_NOREUSE macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NORMAL
#error "sys/fcntl.h:POSIX_FADV_NORMAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_RANDOM
#error "sys/fcntl.h:POSIX_FADV_RANDOM macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_SEQUENTIAL
#error "sys/fcntl.h:POSIX_FADV_SEQUENTIAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_WILLNEED
#error "sys/fcntl.h:POSIX_FADV_WILLNEED macro is missing from libc-shim"
#endif

#ifndef RWF_WRITE_LIFE_NOT_SET
#error "sys/fcntl.h:RWF_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_EXTREME
#error "sys/fcntl.h:RWH_WRITE_LIFE_EXTREME macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_LONG
#error "sys/fcntl.h:RWH_WRITE_LIFE_LONG macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_MEDIUM
#error "sys/fcntl.h:RWH_WRITE_LIFE_MEDIUM macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NONE
#error "sys/fcntl.h:RWH_WRITE_LIFE_NONE macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NOT_SET
#error "sys/fcntl.h:RWH_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_SHORT
#error "sys/fcntl.h:RWH_WRITE_LIFE_SHORT macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_GIFT
#error "sys/fcntl.h:SPLICE_F_GIFT macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MORE
#error "sys/fcntl.h:SPLICE_F_MORE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MOVE
#error "sys/fcntl.h:SPLICE_F_MOVE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_NONBLOCK
#error "sys/fcntl.h:SPLICE_F_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_AFTER
#error "sys/fcntl.h:SYNC_FILE_RANGE_WAIT_AFTER macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_BEFORE
#error "sys/fcntl.h:SYNC_FILE_RANGE_WAIT_BEFORE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE
#error "sys/fcntl.h:SYNC_FILE_RANGE_WRITE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE_AND_WAIT
#error "sys/fcntl.h:SYNC_FILE_RANGE_WRITE_AND_WAIT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
