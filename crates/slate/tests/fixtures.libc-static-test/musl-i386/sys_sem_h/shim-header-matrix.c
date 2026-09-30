#include <sys/sem.h>

typedef struct ipc_perm slate_oracle_struct_semid_ds_sem_perm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->sem_perm), slate_oracle_struct_semid_ds_sem_perm), "struct semid_ds.sem_perm field type differs from oracle");

typedef unsigned long slate_oracle_struct_semid_ds___sem_otime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__sem_otime_lo), slate_oracle_struct_semid_ds___sem_otime_lo), "struct semid_ds.__sem_otime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_semid_ds___sem_otime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__sem_otime_hi), slate_oracle_struct_semid_ds___sem_otime_hi), "struct semid_ds.__sem_otime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_semid_ds___sem_ctime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__sem_ctime_lo), slate_oracle_struct_semid_ds___sem_ctime_lo), "struct semid_ds.__sem_ctime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_semid_ds___sem_ctime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__sem_ctime_hi), slate_oracle_struct_semid_ds___sem_ctime_hi), "struct semid_ds.__sem_ctime_hi field type differs from oracle");

typedef unsigned short slate_oracle_struct_semid_ds_sem_nsems;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->sem_nsems), slate_oracle_struct_semid_ds_sem_nsems), "struct semid_ds.sem_nsems field type differs from oracle");

typedef long slate_oracle_struct_semid_ds___unused3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__unused3), slate_oracle_struct_semid_ds___unused3), "struct semid_ds.__unused3 field type differs from oracle");

typedef long slate_oracle_struct_semid_ds___unused4;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->__unused4), slate_oracle_struct_semid_ds___unused4), "struct semid_ds.__unused4 field type differs from oracle");

typedef long long slate_oracle_struct_semid_ds_sem_otime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->sem_otime), slate_oracle_struct_semid_ds_sem_otime), "struct semid_ds.sem_otime field type differs from oracle");

typedef long long slate_oracle_struct_semid_ds_sem_ctime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct semid_ds *)0)->sem_ctime), slate_oracle_struct_semid_ds_sem_ctime), "struct semid_ds.sem_ctime field type differs from oracle");

_Static_assert(sizeof(struct seminfo) == 40, "struct seminfo size differs from oracle");

_Static_assert(_Alignof(struct seminfo) == 4, "struct seminfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semmap) == 0, "struct seminfo.semmap offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semmap;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semmap), slate_oracle_struct_seminfo_semmap), "struct seminfo.semmap field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semmni) == 4, "struct seminfo.semmni offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semmni;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semmni), slate_oracle_struct_seminfo_semmni), "struct seminfo.semmni field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semmns) == 8, "struct seminfo.semmns offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semmns;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semmns), slate_oracle_struct_seminfo_semmns), "struct seminfo.semmns field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semmnu) == 12, "struct seminfo.semmnu offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semmnu;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semmnu), slate_oracle_struct_seminfo_semmnu), "struct seminfo.semmnu field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semmsl) == 16, "struct seminfo.semmsl offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semmsl;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semmsl), slate_oracle_struct_seminfo_semmsl), "struct seminfo.semmsl field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semopm) == 20, "struct seminfo.semopm offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semopm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semopm), slate_oracle_struct_seminfo_semopm), "struct seminfo.semopm field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semume) == 24, "struct seminfo.semume offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semume;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semume), slate_oracle_struct_seminfo_semume), "struct seminfo.semume field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semusz) == 28, "struct seminfo.semusz offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semusz;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semusz), slate_oracle_struct_seminfo_semusz), "struct seminfo.semusz field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semvmx) == 32, "struct seminfo.semvmx offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semvmx;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semvmx), slate_oracle_struct_seminfo_semvmx), "struct seminfo.semvmx field type differs from oracle");

_Static_assert(__builtin_offsetof(struct seminfo, semaem) == 36, "struct seminfo.semaem offset differs from oracle");

typedef int slate_oracle_struct_seminfo_semaem;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct seminfo *)0)->semaem), slate_oracle_struct_seminfo_semaem), "struct seminfo.semaem field type differs from oracle");

#ifndef GETALL
#error "sys/sem.h:GETALL macro is missing from libc-shim"
#endif

#ifndef GETNCNT
#error "sys/sem.h:GETNCNT macro is missing from libc-shim"
#endif

#ifndef GETPID
#error "sys/sem.h:GETPID macro is missing from libc-shim"
#endif

#ifndef GETVAL
#error "sys/sem.h:GETVAL macro is missing from libc-shim"
#endif

#ifndef GETZCNT
#error "sys/sem.h:GETZCNT macro is missing from libc-shim"
#endif

#ifndef SEM_INFO
#error "sys/sem.h:SEM_INFO macro is missing from libc-shim"
#endif

#ifndef SEM_STAT
#error "sys/sem.h:SEM_STAT macro is missing from libc-shim"
#endif

#ifndef SEM_STAT_ANY
#error "sys/sem.h:SEM_STAT_ANY macro is missing from libc-shim"
#endif

#ifndef SEM_UNDO
#error "sys/sem.h:SEM_UNDO macro is missing from libc-shim"
#endif

#ifndef SETALL
#error "sys/sem.h:SETALL macro is missing from libc-shim"
#endif

#ifndef SETVAL
#error "sys/sem.h:SETVAL macro is missing from libc-shim"
#endif

int main(void) { return 0; }
