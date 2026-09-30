#include <dirent.h>

typedef unsigned short slate_oracle_typedef_reclen_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_reclen_t, reclen_t), "typedef reclen_t differs from oracle");

_Static_assert(sizeof(struct dirent) == 280, "struct dirent size differs from oracle");

_Static_assert(_Alignof(struct dirent) == 8, "struct dirent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct dirent, d_ino) == 0, "struct dirent.d_ino offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dirent_d_ino;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dirent *)0)->d_ino), slate_oracle_struct_dirent_d_ino), "struct dirent.d_ino field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dirent, d_off) == 8, "struct dirent.d_off offset differs from oracle");

typedef long long slate_oracle_struct_dirent_d_off;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dirent *)0)->d_off), slate_oracle_struct_dirent_d_off), "struct dirent.d_off field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dirent, d_reclen) == 16, "struct dirent.d_reclen offset differs from oracle");

typedef unsigned short slate_oracle_struct_dirent_d_reclen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dirent *)0)->d_reclen), slate_oracle_struct_dirent_d_reclen), "struct dirent.d_reclen field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dirent, d_type) == 18, "struct dirent.d_type offset differs from oracle");

typedef unsigned char slate_oracle_struct_dirent_d_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dirent *)0)->d_type), slate_oracle_struct_dirent_d_type), "struct dirent.d_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dirent, d_name) == 19, "struct dirent.d_name offset differs from oracle");

#ifndef DTTOIF
#error "dirent.h:DTTOIF macro is missing from libc-shim"
#endif

#ifndef DT_BLK
#error "dirent.h:DT_BLK macro is missing from libc-shim"
#endif

#ifndef DT_CHR
#error "dirent.h:DT_CHR macro is missing from libc-shim"
#endif

#ifndef DT_DIR
#error "dirent.h:DT_DIR macro is missing from libc-shim"
#endif

#ifndef DT_FIFO
#error "dirent.h:DT_FIFO macro is missing from libc-shim"
#endif

#ifndef DT_LNK
#error "dirent.h:DT_LNK macro is missing from libc-shim"
#endif

#ifndef DT_REG
#error "dirent.h:DT_REG macro is missing from libc-shim"
#endif

#ifndef DT_SOCK
#error "dirent.h:DT_SOCK macro is missing from libc-shim"
#endif

#ifndef DT_UNKNOWN
#error "dirent.h:DT_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef DT_WHT
#error "dirent.h:DT_WHT macro is missing from libc-shim"
#endif

#ifndef IFTODT
#error "dirent.h:IFTODT macro is missing from libc-shim"
#endif

#ifndef d_fileno
#error "dirent.h:d_fileno macro is missing from libc-shim"
#endif

int main(void) { return 0; }
