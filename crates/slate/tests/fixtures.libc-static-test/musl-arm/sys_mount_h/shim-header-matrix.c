#include <sys/mount.h>

extern int slate_oracle_mount(const char *, const char *, const char *, unsigned long, const void *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mount), __typeof__(mount)),
    "sys/mount.h:mount declaration differs from oracle");

static __typeof__(mount) *const slate_reference_mount = &mount;

#ifndef BLKBSZGET
#error "sys/mount.h:BLKBSZGET macro is missing from libc-shim"
#endif

#ifndef BLKBSZSET
#error "sys/mount.h:BLKBSZSET macro is missing from libc-shim"
#endif

#ifndef BLKFLSBUF
#error "sys/mount.h:BLKFLSBUF macro is missing from libc-shim"
#endif

#ifndef BLKFRAGET
#error "sys/mount.h:BLKFRAGET macro is missing from libc-shim"
#endif

#ifndef BLKFRASET
#error "sys/mount.h:BLKFRASET macro is missing from libc-shim"
#endif

#ifndef BLKGETSIZE
#error "sys/mount.h:BLKGETSIZE macro is missing from libc-shim"
#endif

#ifndef BLKGETSIZE64
#error "sys/mount.h:BLKGETSIZE64 macro is missing from libc-shim"
#endif

#ifndef BLKRAGET
#error "sys/mount.h:BLKRAGET macro is missing from libc-shim"
#endif

#ifndef BLKRASET
#error "sys/mount.h:BLKRASET macro is missing from libc-shim"
#endif

#ifndef BLKROGET
#error "sys/mount.h:BLKROGET macro is missing from libc-shim"
#endif

#ifndef BLKROSET
#error "sys/mount.h:BLKROSET macro is missing from libc-shim"
#endif

#ifndef BLKRRPART
#error "sys/mount.h:BLKRRPART macro is missing from libc-shim"
#endif

#ifndef BLKSECTGET
#error "sys/mount.h:BLKSECTGET macro is missing from libc-shim"
#endif

#ifndef BLKSECTSET
#error "sys/mount.h:BLKSECTSET macro is missing from libc-shim"
#endif

#ifndef BLKSSZGET
#error "sys/mount.h:BLKSSZGET macro is missing from libc-shim"
#endif

#ifndef MNT_DETACH
#error "sys/mount.h:MNT_DETACH macro is missing from libc-shim"
#endif

#ifndef MNT_EXPIRE
#error "sys/mount.h:MNT_EXPIRE macro is missing from libc-shim"
#endif

#ifndef MNT_FORCE
#error "sys/mount.h:MNT_FORCE macro is missing from libc-shim"
#endif

#ifndef MS_ACTIVE
#error "sys/mount.h:MS_ACTIVE macro is missing from libc-shim"
#endif

#ifndef MS_BIND
#error "sys/mount.h:MS_BIND macro is missing from libc-shim"
#endif

#ifndef MS_BORN
#error "sys/mount.h:MS_BORN macro is missing from libc-shim"
#endif

#ifndef MS_DIRSYNC
#error "sys/mount.h:MS_DIRSYNC macro is missing from libc-shim"
#endif

#ifndef MS_I_VERSION
#error "sys/mount.h:MS_I_VERSION macro is missing from libc-shim"
#endif

#ifndef MS_KERNMOUNT
#error "sys/mount.h:MS_KERNMOUNT macro is missing from libc-shim"
#endif

#ifndef MS_LAZYTIME
#error "sys/mount.h:MS_LAZYTIME macro is missing from libc-shim"
#endif

#ifndef MS_MANDLOCK
#error "sys/mount.h:MS_MANDLOCK macro is missing from libc-shim"
#endif

#ifndef MS_MGC_MSK
#error "sys/mount.h:MS_MGC_MSK macro is missing from libc-shim"
#endif

#ifndef MS_MGC_VAL
#error "sys/mount.h:MS_MGC_VAL macro is missing from libc-shim"
#endif

#ifndef MS_MOVE
#error "sys/mount.h:MS_MOVE macro is missing from libc-shim"
#endif

#ifndef MS_NOATIME
#error "sys/mount.h:MS_NOATIME macro is missing from libc-shim"
#endif

#ifndef MS_NODEV
#error "sys/mount.h:MS_NODEV macro is missing from libc-shim"
#endif

#ifndef MS_NODIRATIME
#error "sys/mount.h:MS_NODIRATIME macro is missing from libc-shim"
#endif

#ifndef MS_NOEXEC
#error "sys/mount.h:MS_NOEXEC macro is missing from libc-shim"
#endif

#ifndef MS_NOREMOTELOCK
#error "sys/mount.h:MS_NOREMOTELOCK macro is missing from libc-shim"
#endif

#ifndef MS_NOSEC
#error "sys/mount.h:MS_NOSEC macro is missing from libc-shim"
#endif

#ifndef MS_NOSUID
#error "sys/mount.h:MS_NOSUID macro is missing from libc-shim"
#endif

#ifndef MS_NOSYMFOLLOW
#error "sys/mount.h:MS_NOSYMFOLLOW macro is missing from libc-shim"
#endif

#ifndef MS_NOUSER
#error "sys/mount.h:MS_NOUSER macro is missing from libc-shim"
#endif

#ifndef MS_POSIXACL
#error "sys/mount.h:MS_POSIXACL macro is missing from libc-shim"
#endif

#ifndef MS_PRIVATE
#error "sys/mount.h:MS_PRIVATE macro is missing from libc-shim"
#endif

#ifndef MS_RDONLY
#error "sys/mount.h:MS_RDONLY macro is missing from libc-shim"
#endif

#ifndef MS_REC
#error "sys/mount.h:MS_REC macro is missing from libc-shim"
#endif

#ifndef MS_RELATIME
#error "sys/mount.h:MS_RELATIME macro is missing from libc-shim"
#endif

#ifndef MS_REMOUNT
#error "sys/mount.h:MS_REMOUNT macro is missing from libc-shim"
#endif

#ifndef MS_RMT_MASK
#error "sys/mount.h:MS_RMT_MASK macro is missing from libc-shim"
#endif

#ifndef MS_SHARED
#error "sys/mount.h:MS_SHARED macro is missing from libc-shim"
#endif

#ifndef MS_SILENT
#error "sys/mount.h:MS_SILENT macro is missing from libc-shim"
#endif

#ifndef MS_SLAVE
#error "sys/mount.h:MS_SLAVE macro is missing from libc-shim"
#endif

#ifndef MS_STRICTATIME
#error "sys/mount.h:MS_STRICTATIME macro is missing from libc-shim"
#endif

#ifndef MS_SYNCHRONOUS
#error "sys/mount.h:MS_SYNCHRONOUS macro is missing from libc-shim"
#endif

#ifndef MS_UNBINDABLE
#error "sys/mount.h:MS_UNBINDABLE macro is missing from libc-shim"
#endif

#ifndef UMOUNT_NOFOLLOW
#error "sys/mount.h:UMOUNT_NOFOLLOW macro is missing from libc-shim"
#endif

int main(void) { return 0; }
