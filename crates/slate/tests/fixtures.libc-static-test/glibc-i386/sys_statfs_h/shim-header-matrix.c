#include <sys/statfs.h>

extern int slate_oracle_statfs(const char *, struct statfs *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_statfs), __typeof__(statfs)),
    "sys/statfs.h:statfs declaration differs from oracle");

static __typeof__(statfs) *const slate_reference_statfs = &statfs;

_Static_assert(sizeof(struct statfs) == 64, "struct statfs size differs from oracle");

_Static_assert(_Alignof(struct statfs) == 4, "struct statfs alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_type) == 0, "struct statfs.f_type offset differs from oracle");

typedef int slate_oracle_struct_statfs_f_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_type), slate_oracle_struct_statfs_f_type), "struct statfs.f_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_bsize) == 4, "struct statfs.f_bsize offset differs from oracle");

typedef int slate_oracle_struct_statfs_f_bsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_bsize), slate_oracle_struct_statfs_f_bsize), "struct statfs.f_bsize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_blocks) == 8, "struct statfs.f_blocks offset differs from oracle");

typedef unsigned long slate_oracle_struct_statfs_f_blocks;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_blocks), slate_oracle_struct_statfs_f_blocks), "struct statfs.f_blocks field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_bfree) == 12, "struct statfs.f_bfree offset differs from oracle");

typedef unsigned long slate_oracle_struct_statfs_f_bfree;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_bfree), slate_oracle_struct_statfs_f_bfree), "struct statfs.f_bfree field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_bavail) == 16, "struct statfs.f_bavail offset differs from oracle");

typedef unsigned long slate_oracle_struct_statfs_f_bavail;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_bavail), slate_oracle_struct_statfs_f_bavail), "struct statfs.f_bavail field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_files) == 20, "struct statfs.f_files offset differs from oracle");

typedef unsigned long slate_oracle_struct_statfs_f_files;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_files), slate_oracle_struct_statfs_f_files), "struct statfs.f_files field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_ffree) == 24, "struct statfs.f_ffree offset differs from oracle");

typedef unsigned long slate_oracle_struct_statfs_f_ffree;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_ffree), slate_oracle_struct_statfs_f_ffree), "struct statfs.f_ffree field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_fsid) == 28, "struct statfs.f_fsid offset differs from oracle");

typedef struct __fsid_t slate_oracle_struct_statfs_f_fsid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_fsid), slate_oracle_struct_statfs_f_fsid), "struct statfs.f_fsid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_namelen) == 36, "struct statfs.f_namelen offset differs from oracle");

typedef int slate_oracle_struct_statfs_f_namelen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_namelen), slate_oracle_struct_statfs_f_namelen), "struct statfs.f_namelen field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_frsize) == 40, "struct statfs.f_frsize offset differs from oracle");

typedef int slate_oracle_struct_statfs_f_frsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_frsize), slate_oracle_struct_statfs_f_frsize), "struct statfs.f_frsize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_flags) == 44, "struct statfs.f_flags offset differs from oracle");

typedef int slate_oracle_struct_statfs_f_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct statfs *)0)->f_flags), slate_oracle_struct_statfs_f_flags), "struct statfs.f_flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct statfs, f_spare) == 48, "struct statfs.f_spare offset differs from oracle");

int main(void) { return 0; }
