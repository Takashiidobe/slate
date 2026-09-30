#include <aio.h>

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

int main(void) { return 0; }
