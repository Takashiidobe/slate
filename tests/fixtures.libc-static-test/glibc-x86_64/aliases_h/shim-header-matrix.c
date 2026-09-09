#include <aliases.h>

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

typedef int slate_oracle_typedef_clockid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clockid_t, clockid_t), "typedef clockid_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_pthread_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_t, pthread_t), "typedef pthread_t differs from oracle");

typedef struct __sigset_t slate_oracle_typedef_sigset_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigset_t, sigset_t), "typedef sigset_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

typedef void * slate_oracle_typedef_timer_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_timer_t, timer_t), "typedef timer_t differs from oracle");

_Static_assert(sizeof(struct aliasent) == 32, "struct aliasent size differs from oracle");

_Static_assert(_Alignof(struct aliasent) == 8, "struct aliasent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_name) == 0, "struct aliasent.alias_name offset differs from oracle");

typedef char * slate_oracle_struct_aliasent_alias_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_name), slate_oracle_struct_aliasent_alias_name), "struct aliasent.alias_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_members_len) == 8, "struct aliasent.alias_members_len offset differs from oracle");

typedef unsigned long slate_oracle_struct_aliasent_alias_members_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_members_len), slate_oracle_struct_aliasent_alias_members_len), "struct aliasent.alias_members_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_members) == 16, "struct aliasent.alias_members offset differs from oracle");

typedef char ** slate_oracle_struct_aliasent_alias_members;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_members), slate_oracle_struct_aliasent_alias_members), "struct aliasent.alias_members field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_local) == 24, "struct aliasent.alias_local offset differs from oracle");

typedef int slate_oracle_struct_aliasent_alias_local;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_local), slate_oracle_struct_aliasent_alias_local), "struct aliasent.alias_local field type differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(sizeof(struct timeval) == 16, "struct timeval size differs from oracle");

_Static_assert(_Alignof(struct timeval) == 8, "struct timeval alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_sec) == 0, "struct timeval.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_sec), slate_oracle_struct_timeval_tv_sec), "struct timeval.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_usec) == 8, "struct timeval.tv_usec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_usec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_usec), slate_oracle_struct_timeval_tv_usec), "struct timeval.tv_usec field type differs from oracle");

int main(void) { return 0; }
