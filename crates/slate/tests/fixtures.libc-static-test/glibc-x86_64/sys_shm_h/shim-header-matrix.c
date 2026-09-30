#include <sys/shm.h>

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

typedef unsigned long slate_oracle_typedef_shmatt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_shmatt_t, shmatt_t), "typedef shmatt_t differs from oracle");

_Static_assert(sizeof(struct shminfo) == 72, "struct shminfo size differs from oracle");

_Static_assert(_Alignof(struct shminfo) == 8, "struct shminfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, shmmax) == 0, "struct shminfo.shmmax offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo_shmmax;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->shmmax), slate_oracle_struct_shminfo_shmmax), "struct shminfo.shmmax field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, shmmin) == 8, "struct shminfo.shmmin offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo_shmmin;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->shmmin), slate_oracle_struct_shminfo_shmmin), "struct shminfo.shmmin field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, shmmni) == 16, "struct shminfo.shmmni offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo_shmmni;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->shmmni), slate_oracle_struct_shminfo_shmmni), "struct shminfo.shmmni field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, shmseg) == 24, "struct shminfo.shmseg offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo_shmseg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->shmseg), slate_oracle_struct_shminfo_shmseg), "struct shminfo.shmseg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, shmall) == 32, "struct shminfo.shmall offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo_shmall;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->shmall), slate_oracle_struct_shminfo_shmall), "struct shminfo.shmall field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, __glibc_reserved1) == 40, "struct shminfo.__glibc_reserved1 offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo___glibc_reserved1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->__glibc_reserved1), slate_oracle_struct_shminfo___glibc_reserved1), "struct shminfo.__glibc_reserved1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, __glibc_reserved2) == 48, "struct shminfo.__glibc_reserved2 offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo___glibc_reserved2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->__glibc_reserved2), slate_oracle_struct_shminfo___glibc_reserved2), "struct shminfo.__glibc_reserved2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, __glibc_reserved3) == 56, "struct shminfo.__glibc_reserved3 offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo___glibc_reserved3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->__glibc_reserved3), slate_oracle_struct_shminfo___glibc_reserved3), "struct shminfo.__glibc_reserved3 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct shminfo, __glibc_reserved4) == 64, "struct shminfo.__glibc_reserved4 offset differs from oracle");

typedef unsigned long slate_oracle_struct_shminfo___glibc_reserved4;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shminfo *)0)->__glibc_reserved4), slate_oracle_struct_shminfo___glibc_reserved4), "struct shminfo.__glibc_reserved4 field type differs from oracle");

#ifndef SHM_DEST
#error "sys/shm.h:SHM_DEST macro is missing from libc-shim"
#endif

#ifndef SHM_EXEC
#error "sys/shm.h:SHM_EXEC macro is missing from libc-shim"
#endif

#ifndef SHM_HUGETLB
#error "sys/shm.h:SHM_HUGETLB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_16GB
#error "sys/shm.h:SHM_HUGE_16GB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_16KB
#error "sys/shm.h:SHM_HUGE_16KB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_16MB
#error "sys/shm.h:SHM_HUGE_16MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_1GB
#error "sys/shm.h:SHM_HUGE_1GB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_1MB
#error "sys/shm.h:SHM_HUGE_1MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_256MB
#error "sys/shm.h:SHM_HUGE_256MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_2GB
#error "sys/shm.h:SHM_HUGE_2GB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_2MB
#error "sys/shm.h:SHM_HUGE_2MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_32MB
#error "sys/shm.h:SHM_HUGE_32MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_512KB
#error "sys/shm.h:SHM_HUGE_512KB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_512MB
#error "sys/shm.h:SHM_HUGE_512MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_64KB
#error "sys/shm.h:SHM_HUGE_64KB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_8MB
#error "sys/shm.h:SHM_HUGE_8MB macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_MASK
#error "sys/shm.h:SHM_HUGE_MASK macro is missing from libc-shim"
#endif

#ifndef SHM_HUGE_SHIFT
#error "sys/shm.h:SHM_HUGE_SHIFT macro is missing from libc-shim"
#endif

#ifndef SHM_INFO
#error "sys/shm.h:SHM_INFO macro is missing from libc-shim"
#endif

#ifndef SHM_LOCK
#error "sys/shm.h:SHM_LOCK macro is missing from libc-shim"
#endif

#ifndef SHM_LOCKED
#error "sys/shm.h:SHM_LOCKED macro is missing from libc-shim"
#endif

#ifndef SHM_NORESERVE
#error "sys/shm.h:SHM_NORESERVE macro is missing from libc-shim"
#endif

#ifndef SHM_R
#error "sys/shm.h:SHM_R macro is missing from libc-shim"
#endif

#ifndef SHM_RDONLY
#error "sys/shm.h:SHM_RDONLY macro is missing from libc-shim"
#endif

#ifndef SHM_REMAP
#error "sys/shm.h:SHM_REMAP macro is missing from libc-shim"
#endif

#ifndef SHM_RND
#error "sys/shm.h:SHM_RND macro is missing from libc-shim"
#endif

#ifndef SHM_STAT
#error "sys/shm.h:SHM_STAT macro is missing from libc-shim"
#endif

#ifndef SHM_STAT_ANY
#error "sys/shm.h:SHM_STAT_ANY macro is missing from libc-shim"
#endif

#ifndef SHM_UNLOCK
#error "sys/shm.h:SHM_UNLOCK macro is missing from libc-shim"
#endif

#ifndef SHM_W
#error "sys/shm.h:SHM_W macro is missing from libc-shim"
#endif

int main(void) { return 0; }
