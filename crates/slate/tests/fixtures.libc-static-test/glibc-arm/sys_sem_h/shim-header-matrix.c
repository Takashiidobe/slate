#include <sys/sem.h>

_Static_assert(sizeof(struct sembuf) == 6, "struct sembuf size differs from oracle");

_Static_assert(_Alignof(struct sembuf) == 2, "struct sembuf alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sembuf, sem_num) == 0, "struct sembuf.sem_num offset differs from oracle");

typedef unsigned short slate_oracle_struct_sembuf_sem_num;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sembuf *)0)->sem_num), slate_oracle_struct_sembuf_sem_num), "struct sembuf.sem_num field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sembuf, sem_op) == 2, "struct sembuf.sem_op offset differs from oracle");

typedef short slate_oracle_struct_sembuf_sem_op;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sembuf *)0)->sem_op), slate_oracle_struct_sembuf_sem_op), "struct sembuf.sem_op field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sembuf, sem_flg) == 4, "struct sembuf.sem_flg offset differs from oracle");

typedef short slate_oracle_struct_sembuf_sem_flg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sembuf *)0)->sem_flg), slate_oracle_struct_sembuf_sem_flg), "struct sembuf.sem_flg field type differs from oracle");

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
