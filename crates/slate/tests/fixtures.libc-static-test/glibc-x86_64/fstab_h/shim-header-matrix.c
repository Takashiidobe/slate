#include <fstab.h>

_Static_assert(sizeof(struct fstab) == 48, "struct fstab size differs from oracle");

_Static_assert(_Alignof(struct fstab) == 8, "struct fstab alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_spec) == 0, "struct fstab.fs_spec offset differs from oracle");

typedef char * slate_oracle_struct_fstab_fs_spec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_spec), slate_oracle_struct_fstab_fs_spec), "struct fstab.fs_spec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_file) == 8, "struct fstab.fs_file offset differs from oracle");

typedef char * slate_oracle_struct_fstab_fs_file;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_file), slate_oracle_struct_fstab_fs_file), "struct fstab.fs_file field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_vfstype) == 16, "struct fstab.fs_vfstype offset differs from oracle");

typedef char * slate_oracle_struct_fstab_fs_vfstype;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_vfstype), slate_oracle_struct_fstab_fs_vfstype), "struct fstab.fs_vfstype field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_mntops) == 24, "struct fstab.fs_mntops offset differs from oracle");

typedef char * slate_oracle_struct_fstab_fs_mntops;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_mntops), slate_oracle_struct_fstab_fs_mntops), "struct fstab.fs_mntops field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_type) == 32, "struct fstab.fs_type offset differs from oracle");

typedef const char * slate_oracle_struct_fstab_fs_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_type), slate_oracle_struct_fstab_fs_type), "struct fstab.fs_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_freq) == 40, "struct fstab.fs_freq offset differs from oracle");

typedef int slate_oracle_struct_fstab_fs_freq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_freq), slate_oracle_struct_fstab_fs_freq), "struct fstab.fs_freq field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fstab, fs_passno) == 44, "struct fstab.fs_passno offset differs from oracle");

typedef int slate_oracle_struct_fstab_fs_passno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fstab *)0)->fs_passno), slate_oracle_struct_fstab_fs_passno), "struct fstab.fs_passno field type differs from oracle");

#ifndef FSTAB
#error "fstab.h:FSTAB macro is missing from libc-shim"
#endif

#ifndef FSTAB_RO
#error "fstab.h:FSTAB_RO macro is missing from libc-shim"
#endif

#ifndef FSTAB_RQ
#error "fstab.h:FSTAB_RQ macro is missing from libc-shim"
#endif

#ifndef FSTAB_RW
#error "fstab.h:FSTAB_RW macro is missing from libc-shim"
#endif

#ifndef FSTAB_SW
#error "fstab.h:FSTAB_SW macro is missing from libc-shim"
#endif

#ifndef FSTAB_XX
#error "fstab.h:FSTAB_XX macro is missing from libc-shim"
#endif

int main(void) { return 0; }
