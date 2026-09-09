#include <a.out.h>

_Static_assert(sizeof(struct exec) == 40, "struct exec size differs from oracle");

_Static_assert(_Alignof(struct exec) == 8, "struct exec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_info) == 0, "struct exec.a_info offset differs from oracle");

typedef unsigned long slate_oracle_struct_exec_a_info;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_info), slate_oracle_struct_exec_a_info), "struct exec.a_info field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_text) == 8, "struct exec.a_text offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_text;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_text), slate_oracle_struct_exec_a_text), "struct exec.a_text field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_data) == 12, "struct exec.a_data offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_data), slate_oracle_struct_exec_a_data), "struct exec.a_data field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_bss) == 16, "struct exec.a_bss offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_bss;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_bss), slate_oracle_struct_exec_a_bss), "struct exec.a_bss field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_syms) == 20, "struct exec.a_syms offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_syms;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_syms), slate_oracle_struct_exec_a_syms), "struct exec.a_syms field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_entry) == 24, "struct exec.a_entry offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_entry;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_entry), slate_oracle_struct_exec_a_entry), "struct exec.a_entry field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_trsize) == 28, "struct exec.a_trsize offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_trsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_trsize), slate_oracle_struct_exec_a_trsize), "struct exec.a_trsize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct exec, a_drsize) == 32, "struct exec.a_drsize offset differs from oracle");

typedef unsigned int slate_oracle_struct_exec_a_drsize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct exec *)0)->a_drsize), slate_oracle_struct_exec_a_drsize), "struct exec.a_drsize field type differs from oracle");

#ifndef CMAGIC
#error "a.out.h:CMAGIC macro is missing from libc-shim"
#endif

#ifndef NMAGIC
#error "a.out.h:NMAGIC macro is missing from libc-shim"
#endif

#ifndef N_ABS
#error "a.out.h:N_ABS macro is missing from libc-shim"
#endif

#ifndef N_BADMAG
#error "a.out.h:N_BADMAG macro is missing from libc-shim"
#endif

#ifndef N_BSS
#error "a.out.h:N_BSS macro is missing from libc-shim"
#endif

#ifndef N_BSSADDR
#error "a.out.h:N_BSSADDR macro is missing from libc-shim"
#endif

#ifndef N_DATA
#error "a.out.h:N_DATA macro is missing from libc-shim"
#endif

#ifndef N_DATADDR
#error "a.out.h:N_DATADDR macro is missing from libc-shim"
#endif

#ifndef N_DATOFF
#error "a.out.h:N_DATOFF macro is missing from libc-shim"
#endif

#ifndef N_DRELOFF
#error "a.out.h:N_DRELOFF macro is missing from libc-shim"
#endif

#ifndef N_DRSIZE
#error "a.out.h:N_DRSIZE macro is missing from libc-shim"
#endif

#ifndef N_EXT
#error "a.out.h:N_EXT macro is missing from libc-shim"
#endif

#ifndef N_FLAGS
#error "a.out.h:N_FLAGS macro is missing from libc-shim"
#endif

#ifndef N_FN
#error "a.out.h:N_FN macro is missing from libc-shim"
#endif

#ifndef N_INDR
#error "a.out.h:N_INDR macro is missing from libc-shim"
#endif

#ifndef N_MACHTYPE
#error "a.out.h:N_MACHTYPE macro is missing from libc-shim"
#endif

#ifndef N_MAGIC
#error "a.out.h:N_MAGIC macro is missing from libc-shim"
#endif

#ifndef N_SETA
#error "a.out.h:N_SETA macro is missing from libc-shim"
#endif

#ifndef N_SETB
#error "a.out.h:N_SETB macro is missing from libc-shim"
#endif

#ifndef N_SETD
#error "a.out.h:N_SETD macro is missing from libc-shim"
#endif

#ifndef N_SETT
#error "a.out.h:N_SETT macro is missing from libc-shim"
#endif

#ifndef N_SETV
#error "a.out.h:N_SETV macro is missing from libc-shim"
#endif

#ifndef N_SET_FLAGS
#error "a.out.h:N_SET_FLAGS macro is missing from libc-shim"
#endif

#ifndef N_SET_INFO
#error "a.out.h:N_SET_INFO macro is missing from libc-shim"
#endif

#ifndef N_SET_MACHTYPE
#error "a.out.h:N_SET_MACHTYPE macro is missing from libc-shim"
#endif

#ifndef N_SET_MAGIC
#error "a.out.h:N_SET_MAGIC macro is missing from libc-shim"
#endif

#ifndef N_STAB
#error "a.out.h:N_STAB macro is missing from libc-shim"
#endif

#ifndef N_STROFF
#error "a.out.h:N_STROFF macro is missing from libc-shim"
#endif

#ifndef N_SYMOFF
#error "a.out.h:N_SYMOFF macro is missing from libc-shim"
#endif

#ifndef N_SYMSIZE
#error "a.out.h:N_SYMSIZE macro is missing from libc-shim"
#endif

#ifndef N_TEXT
#error "a.out.h:N_TEXT macro is missing from libc-shim"
#endif

#ifndef N_TRELOFF
#error "a.out.h:N_TRELOFF macro is missing from libc-shim"
#endif

#ifndef N_TRSIZE
#error "a.out.h:N_TRSIZE macro is missing from libc-shim"
#endif

#ifndef N_TXTADDR
#error "a.out.h:N_TXTADDR macro is missing from libc-shim"
#endif

#ifndef N_TXTOFF
#error "a.out.h:N_TXTOFF macro is missing from libc-shim"
#endif

#ifndef N_TYPE
#error "a.out.h:N_TYPE macro is missing from libc-shim"
#endif

#ifndef N_UNDF
#error "a.out.h:N_UNDF macro is missing from libc-shim"
#endif

#ifndef OMAGIC
#error "a.out.h:OMAGIC macro is missing from libc-shim"
#endif

#ifndef QMAGIC
#error "a.out.h:QMAGIC macro is missing from libc-shim"
#endif

#ifndef SEGMENT_SIZE
#error "a.out.h:SEGMENT_SIZE macro is missing from libc-shim"
#endif

#ifndef ZMAGIC
#error "a.out.h:ZMAGIC macro is missing from libc-shim"
#endif

int main(void) { return 0; }
