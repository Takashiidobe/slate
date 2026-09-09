#include <aio.h>

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

_Static_assert(__builtin_offsetof(struct aiocb, aio_fildes) == 0, "struct aiocb.aio_fildes offset differs from oracle");

typedef int slate_oracle_struct_aiocb_aio_fildes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_fildes), slate_oracle_struct_aiocb_aio_fildes), "struct aiocb.aio_fildes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aiocb, aio_lio_opcode) == 4, "struct aiocb.aio_lio_opcode offset differs from oracle");

typedef int slate_oracle_struct_aiocb_aio_lio_opcode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_lio_opcode), slate_oracle_struct_aiocb_aio_lio_opcode), "struct aiocb.aio_lio_opcode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aiocb, aio_reqprio) == 8, "struct aiocb.aio_reqprio offset differs from oracle");

typedef int slate_oracle_struct_aiocb_aio_reqprio;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_reqprio), slate_oracle_struct_aiocb_aio_reqprio), "struct aiocb.aio_reqprio field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aiocb, aio_buf) == 16, "struct aiocb.aio_buf offset differs from oracle");

typedef volatile void * slate_oracle_struct_aiocb_aio_buf;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_buf), slate_oracle_struct_aiocb_aio_buf), "struct aiocb.aio_buf field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aiocb, aio_nbytes) == 24, "struct aiocb.aio_nbytes offset differs from oracle");

typedef unsigned long slate_oracle_struct_aiocb_aio_nbytes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_nbytes), slate_oracle_struct_aiocb_aio_nbytes), "struct aiocb.aio_nbytes field type differs from oracle");

typedef struct sigevent slate_oracle_struct_aiocb_aio_sigevent;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_sigevent), slate_oracle_struct_aiocb_aio_sigevent), "struct aiocb.aio_sigevent field type differs from oracle");

typedef struct aiocb * slate_oracle_struct_aiocb___next_prio;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->__next_prio), slate_oracle_struct_aiocb___next_prio), "struct aiocb.__next_prio field type differs from oracle");

typedef int slate_oracle_struct_aiocb___abs_prio;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->__abs_prio), slate_oracle_struct_aiocb___abs_prio), "struct aiocb.__abs_prio field type differs from oracle");

typedef int slate_oracle_struct_aiocb___policy;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->__policy), slate_oracle_struct_aiocb___policy), "struct aiocb.__policy field type differs from oracle");

typedef int slate_oracle_struct_aiocb___error_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->__error_code), slate_oracle_struct_aiocb___error_code), "struct aiocb.__error_code field type differs from oracle");

typedef long slate_oracle_struct_aiocb___return_value;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->__return_value), slate_oracle_struct_aiocb___return_value), "struct aiocb.__return_value field type differs from oracle");

typedef long slate_oracle_struct_aiocb_aio_offset;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aiocb *)0)->aio_offset), slate_oracle_struct_aiocb_aio_offset), "struct aiocb.aio_offset field type differs from oracle");

typedef union sigval slate_oracle_struct_sigevent_sigev_value;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_value), slate_oracle_struct_sigevent_sigev_value), "struct sigevent.sigev_value field type differs from oracle");

typedef int slate_oracle_struct_sigevent_sigev_signo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_signo), slate_oracle_struct_sigevent_sigev_signo), "struct sigevent.sigev_signo field type differs from oracle");

typedef int slate_oracle_struct_sigevent_sigev_notify;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_notify), slate_oracle_struct_sigevent_sigev_notify), "struct sigevent.sigev_notify field type differs from oracle");

_Static_assert(sizeof(union sigval) == 8, "union sigval size differs from oracle");

_Static_assert(_Alignof(union sigval) == 8, "union sigval alignment differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_int) == 0, "union sigval.sival_int offset differs from oracle");

typedef int slate_oracle_union_sigval_sival_int;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_int), slate_oracle_union_sigval_sival_int), "union sigval.sival_int field type differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_ptr) == 0, "union sigval.sival_ptr offset differs from oracle");

typedef void * slate_oracle_union_sigval_sival_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_ptr), slate_oracle_union_sigval_sival_ptr), "union sigval.sival_ptr field type differs from oracle");

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

#ifndef AIO_ALLDONE
#error "aio.h:AIO_ALLDONE macro is missing from libc-shim"
#endif

#ifndef AIO_CANCELED
#error "aio.h:AIO_CANCELED macro is missing from libc-shim"
#endif

#ifndef AIO_NOTCANCELED
#error "aio.h:AIO_NOTCANCELED macro is missing from libc-shim"
#endif

#ifndef LIO_NOP
#error "aio.h:LIO_NOP macro is missing from libc-shim"
#endif

#ifndef LIO_NOWAIT
#error "aio.h:LIO_NOWAIT macro is missing from libc-shim"
#endif

#ifndef LIO_READ
#error "aio.h:LIO_READ macro is missing from libc-shim"
#endif

#ifndef LIO_WAIT
#error "aio.h:LIO_WAIT macro is missing from libc-shim"
#endif

#ifndef LIO_WRITE
#error "aio.h:LIO_WRITE macro is missing from libc-shim"
#endif

#ifndef SIGEV_NONE
#error "aio.h:SIGEV_NONE macro is missing from libc-shim"
#endif

#ifndef SIGEV_SIGNAL
#error "aio.h:SIGEV_SIGNAL macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD
#error "aio.h:SIGEV_THREAD macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD_ID
#error "aio.h:SIGEV_THREAD_ID macro is missing from libc-shim"
#endif

#ifndef sigev_notify_attributes
#error "aio.h:sigev_notify_attributes macro is missing from libc-shim"
#endif

#ifndef sigev_notify_function
#error "aio.h:sigev_notify_function macro is missing from libc-shim"
#endif

int main(void) { return 0; }
