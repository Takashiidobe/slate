#include <sys/shm.h>

typedef unsigned long slate_oracle_typedef_shmatt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_shmatt_t, shmatt_t), "typedef shmatt_t differs from oracle");

typedef struct ipc_perm slate_oracle_struct_shmid_ds_shm_perm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_perm), slate_oracle_struct_shmid_ds_shm_perm), "struct shmid_ds.shm_perm field type differs from oracle");

typedef unsigned int slate_oracle_struct_shmid_ds_shm_segsz;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_segsz), slate_oracle_struct_shmid_ds_shm_segsz), "struct shmid_ds.shm_segsz field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_atime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_atime_lo), slate_oracle_struct_shmid_ds___shm_atime_lo), "struct shmid_ds.__shm_atime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_atime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_atime_hi), slate_oracle_struct_shmid_ds___shm_atime_hi), "struct shmid_ds.__shm_atime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_dtime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_dtime_lo), slate_oracle_struct_shmid_ds___shm_dtime_lo), "struct shmid_ds.__shm_dtime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_dtime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_dtime_hi), slate_oracle_struct_shmid_ds___shm_dtime_hi), "struct shmid_ds.__shm_dtime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_ctime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_ctime_lo), slate_oracle_struct_shmid_ds___shm_ctime_lo), "struct shmid_ds.__shm_ctime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___shm_ctime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__shm_ctime_hi), slate_oracle_struct_shmid_ds___shm_ctime_hi), "struct shmid_ds.__shm_ctime_hi field type differs from oracle");

typedef int slate_oracle_struct_shmid_ds_shm_cpid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_cpid), slate_oracle_struct_shmid_ds_shm_cpid), "struct shmid_ds.shm_cpid field type differs from oracle");

typedef int slate_oracle_struct_shmid_ds_shm_lpid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_lpid), slate_oracle_struct_shmid_ds_shm_lpid), "struct shmid_ds.shm_lpid field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds_shm_nattch;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_nattch), slate_oracle_struct_shmid_ds_shm_nattch), "struct shmid_ds.shm_nattch field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__pad1), slate_oracle_struct_shmid_ds___pad1), "struct shmid_ds.__pad1 field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__pad2), slate_oracle_struct_shmid_ds___pad2), "struct shmid_ds.__pad2 field type differs from oracle");

typedef unsigned long slate_oracle_struct_shmid_ds___pad3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->__pad3), slate_oracle_struct_shmid_ds___pad3), "struct shmid_ds.__pad3 field type differs from oracle");

typedef long long slate_oracle_struct_shmid_ds_shm_atime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_atime), slate_oracle_struct_shmid_ds_shm_atime), "struct shmid_ds.shm_atime field type differs from oracle");

typedef long long slate_oracle_struct_shmid_ds_shm_dtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_dtime), slate_oracle_struct_shmid_ds_shm_dtime), "struct shmid_ds.shm_dtime field type differs from oracle");

typedef long long slate_oracle_struct_shmid_ds_shm_ctime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shmid_ds *)0)->shm_ctime), slate_oracle_struct_shmid_ds_shm_ctime), "struct shmid_ds.shm_ctime field type differs from oracle");

#ifndef SHMLBA
#error "sys/shm.h:SHMLBA macro is missing from libc-shim"
#endif

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
