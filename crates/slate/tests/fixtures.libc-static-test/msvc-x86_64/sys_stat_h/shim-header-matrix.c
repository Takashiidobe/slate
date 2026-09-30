#include <sys/stat.h>

extern int slate_oracle__fstat32(int, struct _stat32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fstat32), __typeof__(_fstat32)),
    "sys/stat.h:_fstat32 declaration differs from oracle");

static __typeof__(_fstat32) *const slate_reference__fstat32 = &_fstat32;

extern int slate_oracle__fstat32i64(int, struct _stat32i64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fstat32i64), __typeof__(_fstat32i64)),
    "sys/stat.h:_fstat32i64 declaration differs from oracle");

static __typeof__(_fstat32i64) *const slate_reference__fstat32i64 = &_fstat32i64;

extern int slate_oracle__fstat64(int, struct _stat64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fstat64), __typeof__(_fstat64)),
    "sys/stat.h:_fstat64 declaration differs from oracle");

static __typeof__(_fstat64) *const slate_reference__fstat64 = &_fstat64;

extern int slate_oracle__fstat64i32(int, struct _stat64i32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fstat64i32), __typeof__(_fstat64i32)),
    "sys/stat.h:_fstat64i32 declaration differs from oracle");

static __typeof__(_fstat64i32) *const slate_reference__fstat64i32 = &_fstat64i32;

extern int slate_oracle__stat32(const char *, struct _stat32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__stat32), __typeof__(_stat32)),
    "sys/stat.h:_stat32 declaration differs from oracle");

static __typeof__(_stat32) *const slate_reference__stat32 = &_stat32;

extern int slate_oracle__stat32i64(const char *, struct _stat32i64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__stat32i64), __typeof__(_stat32i64)),
    "sys/stat.h:_stat32i64 declaration differs from oracle");

static __typeof__(_stat32i64) *const slate_reference__stat32i64 = &_stat32i64;

extern int slate_oracle__stat64(const char *, struct _stat64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__stat64), __typeof__(_stat64)),
    "sys/stat.h:_stat64 declaration differs from oracle");

static __typeof__(_stat64) *const slate_reference__stat64 = &_stat64;

extern int slate_oracle__stat64i32(const char *, struct _stat64i32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__stat64i32), __typeof__(_stat64i32)),
    "sys/stat.h:_stat64i32 declaration differs from oracle");

static __typeof__(_stat64i32) *const slate_reference__stat64i32 = &_stat64i32;

extern int slate_oracle__wstat32(const unsigned short *, struct _stat32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wstat32), __typeof__(_wstat32)),
    "sys/stat.h:_wstat32 declaration differs from oracle");

static __typeof__(_wstat32) *const slate_reference__wstat32 = &_wstat32;

extern int slate_oracle__wstat32i64(const unsigned short *, struct _stat32i64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wstat32i64), __typeof__(_wstat32i64)),
    "sys/stat.h:_wstat32i64 declaration differs from oracle");

static __typeof__(_wstat32i64) *const slate_reference__wstat32i64 = &_wstat32i64;

extern int slate_oracle__wstat64(const unsigned short *, struct _stat64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wstat64), __typeof__(_wstat64)),
    "sys/stat.h:_wstat64 declaration differs from oracle");

static __typeof__(_wstat64) *const slate_reference__wstat64 = &_wstat64;

extern int slate_oracle__wstat64i32(const unsigned short *, struct _stat64i32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wstat64i32), __typeof__(_wstat64i32)),
    "sys/stat.h:_wstat64i32 declaration differs from oracle");

static __typeof__(_wstat64i32) *const slate_reference__wstat64i32 = &_wstat64i32;

_Static_assert(sizeof(struct stat) == 48, "struct stat size differs from oracle");

_Static_assert(_Alignof(struct stat) == 8, "struct stat alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_dev) == 0, "struct stat.st_dev offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_dev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_dev), slate_oracle_struct_stat_st_dev), "struct stat.st_dev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_ino) == 4, "struct stat.st_ino offset differs from oracle");

typedef unsigned short slate_oracle_struct_stat_st_ino;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ino), slate_oracle_struct_stat_st_ino), "struct stat.st_ino field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_mode) == 6, "struct stat.st_mode offset differs from oracle");

typedef unsigned short slate_oracle_struct_stat_st_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mode), slate_oracle_struct_stat_st_mode), "struct stat.st_mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_nlink) == 8, "struct stat.st_nlink offset differs from oracle");

typedef short slate_oracle_struct_stat_st_nlink;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_nlink), slate_oracle_struct_stat_st_nlink), "struct stat.st_nlink field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_uid) == 10, "struct stat.st_uid offset differs from oracle");

typedef short slate_oracle_struct_stat_st_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_uid), slate_oracle_struct_stat_st_uid), "struct stat.st_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_gid) == 12, "struct stat.st_gid offset differs from oracle");

typedef short slate_oracle_struct_stat_st_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_gid), slate_oracle_struct_stat_st_gid), "struct stat.st_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_rdev) == 16, "struct stat.st_rdev offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_rdev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_rdev), slate_oracle_struct_stat_st_rdev), "struct stat.st_rdev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_size) == 20, "struct stat.st_size offset differs from oracle");

typedef long slate_oracle_struct_stat_st_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_size), slate_oracle_struct_stat_st_size), "struct stat.st_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_atime) == 24, "struct stat.st_atime offset differs from oracle");

typedef long long slate_oracle_struct_stat_st_atime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_atime), slate_oracle_struct_stat_st_atime), "struct stat.st_atime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_mtime) == 32, "struct stat.st_mtime offset differs from oracle");

typedef long long slate_oracle_struct_stat_st_mtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mtime), slate_oracle_struct_stat_st_mtime), "struct stat.st_mtime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_ctime) == 40, "struct stat.st_ctime offset differs from oracle");

typedef long long slate_oracle_struct_stat_st_ctime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ctime), slate_oracle_struct_stat_st_ctime), "struct stat.st_ctime field type differs from oracle");

#ifndef S_IEXEC
#error "sys/stat.h:S_IEXEC macro is missing from libc-shim"
#endif

#ifndef S_IFCHR
#error "sys/stat.h:S_IFCHR macro is missing from libc-shim"
#endif

#ifndef S_IFDIR
#error "sys/stat.h:S_IFDIR macro is missing from libc-shim"
#endif

#ifndef S_IFMT
#error "sys/stat.h:S_IFMT macro is missing from libc-shim"
#endif

#ifndef S_IFREG
#error "sys/stat.h:S_IFREG macro is missing from libc-shim"
#endif

#ifndef S_IREAD
#error "sys/stat.h:S_IREAD macro is missing from libc-shim"
#endif

#ifndef S_IWRITE
#error "sys/stat.h:S_IWRITE macro is missing from libc-shim"
#endif

#ifndef _S_IEXEC
#error "sys/stat.h:_S_IEXEC macro is missing from libc-shim"
#endif

#ifndef _S_IFCHR
#error "sys/stat.h:_S_IFCHR macro is missing from libc-shim"
#endif

#ifndef _S_IFDIR
#error "sys/stat.h:_S_IFDIR macro is missing from libc-shim"
#endif

#ifndef _S_IFIFO
#error "sys/stat.h:_S_IFIFO macro is missing from libc-shim"
#endif

#ifndef _S_IFMT
#error "sys/stat.h:_S_IFMT macro is missing from libc-shim"
#endif

#ifndef _S_IFREG
#error "sys/stat.h:_S_IFREG macro is missing from libc-shim"
#endif

#ifndef _S_IREAD
#error "sys/stat.h:_S_IREAD macro is missing from libc-shim"
#endif

#ifndef _S_IWRITE
#error "sys/stat.h:_S_IWRITE macro is missing from libc-shim"
#endif

#ifndef __stat64
#error "sys/stat.h:__stat64 macro is missing from libc-shim"
#endif

#ifndef _fstat
#error "sys/stat.h:_fstat macro is missing from libc-shim"
#endif

#ifndef _fstati64
#error "sys/stat.h:_fstati64 macro is missing from libc-shim"
#endif

#ifndef _stat
#error "sys/stat.h:_stat macro is missing from libc-shim"
#endif

#ifndef _stati64
#error "sys/stat.h:_stati64 macro is missing from libc-shim"
#endif

#ifndef _wstat
#error "sys/stat.h:_wstat macro is missing from libc-shim"
#endif

#ifndef _wstati64
#error "sys/stat.h:_wstati64 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
