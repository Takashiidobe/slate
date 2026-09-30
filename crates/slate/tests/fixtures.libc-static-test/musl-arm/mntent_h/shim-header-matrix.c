#include <mntent.h>

_Static_assert(sizeof(struct mntent) == 24, "struct mntent size differs from oracle");

_Static_assert(_Alignof(struct mntent) == 4, "struct mntent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_fsname) == 0, "struct mntent.mnt_fsname offset differs from oracle");

typedef char * slate_oracle_struct_mntent_mnt_fsname;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_fsname), slate_oracle_struct_mntent_mnt_fsname), "struct mntent.mnt_fsname field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_dir) == 4, "struct mntent.mnt_dir offset differs from oracle");

typedef char * slate_oracle_struct_mntent_mnt_dir;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_dir), slate_oracle_struct_mntent_mnt_dir), "struct mntent.mnt_dir field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_type) == 8, "struct mntent.mnt_type offset differs from oracle");

typedef char * slate_oracle_struct_mntent_mnt_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_type), slate_oracle_struct_mntent_mnt_type), "struct mntent.mnt_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_opts) == 12, "struct mntent.mnt_opts offset differs from oracle");

typedef char * slate_oracle_struct_mntent_mnt_opts;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_opts), slate_oracle_struct_mntent_mnt_opts), "struct mntent.mnt_opts field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_freq) == 16, "struct mntent.mnt_freq offset differs from oracle");

typedef int slate_oracle_struct_mntent_mnt_freq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_freq), slate_oracle_struct_mntent_mnt_freq), "struct mntent.mnt_freq field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mntent, mnt_passno) == 20, "struct mntent.mnt_passno offset differs from oracle");

typedef int slate_oracle_struct_mntent_mnt_passno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mntent *)0)->mnt_passno), slate_oracle_struct_mntent_mnt_passno), "struct mntent.mnt_passno field type differs from oracle");

#ifndef MNTOPT_DEFAULTS
#error "mntent.h:MNTOPT_DEFAULTS macro is missing from libc-shim"
#endif

#ifndef MNTOPT_NOAUTO
#error "mntent.h:MNTOPT_NOAUTO macro is missing from libc-shim"
#endif

#ifndef MNTOPT_NOSUID
#error "mntent.h:MNTOPT_NOSUID macro is missing from libc-shim"
#endif

#ifndef MNTOPT_RO
#error "mntent.h:MNTOPT_RO macro is missing from libc-shim"
#endif

#ifndef MNTOPT_RW
#error "mntent.h:MNTOPT_RW macro is missing from libc-shim"
#endif

#ifndef MNTOPT_SUID
#error "mntent.h:MNTOPT_SUID macro is missing from libc-shim"
#endif

#ifndef MNTTYPE_IGNORE
#error "mntent.h:MNTTYPE_IGNORE macro is missing from libc-shim"
#endif

#ifndef MNTTYPE_NFS
#error "mntent.h:MNTTYPE_NFS macro is missing from libc-shim"
#endif

#ifndef MNTTYPE_SWAP
#error "mntent.h:MNTTYPE_SWAP macro is missing from libc-shim"
#endif

#ifndef MOUNTED
#error "mntent.h:MOUNTED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
