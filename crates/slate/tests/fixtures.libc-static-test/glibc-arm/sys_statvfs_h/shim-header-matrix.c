#include <sys/statvfs.h>

typedef unsigned long slate_oracle_typedef_fsblkcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsblkcnt_t, fsblkcnt_t), "typedef fsblkcnt_t differs from oracle");

_Static_assert(sizeof(struct statvfs) == 72, "struct statvfs size differs from oracle");

_Static_assert(_Alignof(struct statvfs) == 4, "struct statvfs alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_bsize) == 0, "struct statvfs.f_bsize offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_bsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_bsize), slate_oracle_struct_statvfs_f_bsize), "struct statvfs.f_bsize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_frsize) == 4, "struct statvfs.f_frsize offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_frsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_frsize), slate_oracle_struct_statvfs_f_frsize), "struct statvfs.f_frsize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_blocks) == 8, "struct statvfs.f_blocks offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_blocks;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_blocks), slate_oracle_struct_statvfs_f_blocks), "struct statvfs.f_blocks field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_bfree) == 12, "struct statvfs.f_bfree offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_bfree;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_bfree), slate_oracle_struct_statvfs_f_bfree), "struct statvfs.f_bfree field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_bavail) == 16, "struct statvfs.f_bavail offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_bavail;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_bavail), slate_oracle_struct_statvfs_f_bavail), "struct statvfs.f_bavail field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_files) == 20, "struct statvfs.f_files offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_files;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_files), slate_oracle_struct_statvfs_f_files), "struct statvfs.f_files field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_ffree) == 24, "struct statvfs.f_ffree offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_ffree;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_ffree), slate_oracle_struct_statvfs_f_ffree), "struct statvfs.f_ffree field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_favail) == 28, "struct statvfs.f_favail offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_favail;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_favail), slate_oracle_struct_statvfs_f_favail), "struct statvfs.f_favail field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_fsid) == 32, "struct statvfs.f_fsid offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_fsid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_fsid), slate_oracle_struct_statvfs_f_fsid), "struct statvfs.f_fsid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, __f_unused) == 36, "struct statvfs.__f_unused offset differs from oracle");

typedef int slate_oracle_struct_statvfs___f_unused;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->__f_unused), slate_oracle_struct_statvfs___f_unused), "struct statvfs.__f_unused field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_flag) == 40, "struct statvfs.f_flag offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_flag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_flag), slate_oracle_struct_statvfs_f_flag), "struct statvfs.f_flag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_namemax) == 44, "struct statvfs.f_namemax offset differs from oracle");

typedef unsigned long slate_oracle_struct_statvfs_f_namemax;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_namemax), slate_oracle_struct_statvfs_f_namemax), "struct statvfs.f_namemax field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, f_type) == 48, "struct statvfs.f_type offset differs from oracle");

typedef unsigned int slate_oracle_struct_statvfs_f_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statvfs *)0)->f_type), slate_oracle_struct_statvfs_f_type), "struct statvfs.f_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statvfs, __f_spare) == 52, "struct statvfs.__f_spare offset differs from oracle");

#ifndef ST_APPEND
#error "sys/statvfs.h:ST_APPEND macro is missing from libc-shim"
#endif

#ifndef ST_IMMUTABLE
#error "sys/statvfs.h:ST_IMMUTABLE macro is missing from libc-shim"
#endif

#ifndef ST_MANDLOCK
#error "sys/statvfs.h:ST_MANDLOCK macro is missing from libc-shim"
#endif

#ifndef ST_NOATIME
#error "sys/statvfs.h:ST_NOATIME macro is missing from libc-shim"
#endif

#ifndef ST_NODEV
#error "sys/statvfs.h:ST_NODEV macro is missing from libc-shim"
#endif

#ifndef ST_NODIRATIME
#error "sys/statvfs.h:ST_NODIRATIME macro is missing from libc-shim"
#endif

#ifndef ST_NOEXEC
#error "sys/statvfs.h:ST_NOEXEC macro is missing from libc-shim"
#endif

#ifndef ST_NOSUID
#error "sys/statvfs.h:ST_NOSUID macro is missing from libc-shim"
#endif

#ifndef ST_NOSYMFOLLOW
#error "sys/statvfs.h:ST_NOSYMFOLLOW macro is missing from libc-shim"
#endif

#ifndef ST_RDONLY
#error "sys/statvfs.h:ST_RDONLY macro is missing from libc-shim"
#endif

#ifndef ST_RELATIME
#error "sys/statvfs.h:ST_RELATIME macro is missing from libc-shim"
#endif

#ifndef ST_SYNCHRONOUS
#error "sys/statvfs.h:ST_SYNCHRONOUS macro is missing from libc-shim"
#endif

#ifndef ST_WRITE
#error "sys/statvfs.h:ST_WRITE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
