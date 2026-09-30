#include <sys/ipc.h>

extern int slate_oracle_ftok(const char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftok), __typeof__(ftok)),
    "sys/ipc.h:ftok declaration differs from oracle");

static __typeof__(ftok) *const slate_reference_ftok = &ftok;

_Static_assert(sizeof(struct ipc_perm) == 36, "struct ipc_perm size differs from oracle");

_Static_assert(_Alignof(struct ipc_perm) == 4, "struct ipc_perm alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, key) == 0, "struct ipc_perm.key offset differs from oracle");

typedef int slate_oracle_struct_ipc_perm_key;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->key), slate_oracle_struct_ipc_perm_key), "struct ipc_perm.key field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, uid) == 4, "struct ipc_perm.uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_ipc_perm_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->uid), slate_oracle_struct_ipc_perm_uid), "struct ipc_perm.uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, gid) == 8, "struct ipc_perm.gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_ipc_perm_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->gid), slate_oracle_struct_ipc_perm_gid), "struct ipc_perm.gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, cuid) == 12, "struct ipc_perm.cuid offset differs from oracle");

typedef unsigned int slate_oracle_struct_ipc_perm_cuid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->cuid), slate_oracle_struct_ipc_perm_cuid), "struct ipc_perm.cuid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, cgid) == 16, "struct ipc_perm.cgid offset differs from oracle");

typedef unsigned int slate_oracle_struct_ipc_perm_cgid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->cgid), slate_oracle_struct_ipc_perm_cgid), "struct ipc_perm.cgid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, mode) == 20, "struct ipc_perm.mode offset differs from oracle");

typedef unsigned int slate_oracle_struct_ipc_perm_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->mode), slate_oracle_struct_ipc_perm_mode), "struct ipc_perm.mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, seq) == 24, "struct ipc_perm.seq offset differs from oracle");

typedef int slate_oracle_struct_ipc_perm_seq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->seq), slate_oracle_struct_ipc_perm_seq), "struct ipc_perm.seq field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, __pad1) == 28, "struct ipc_perm.__pad1 offset differs from oracle");

typedef long slate_oracle_struct_ipc_perm___pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->__pad1), slate_oracle_struct_ipc_perm___pad1), "struct ipc_perm.__pad1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ipc_perm, __pad2) == 32, "struct ipc_perm.__pad2 offset differs from oracle");

typedef long slate_oracle_struct_ipc_perm___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ipc_perm *)0)->__pad2), slate_oracle_struct_ipc_perm___pad2), "struct ipc_perm.__pad2 field type differs from oracle");

#ifndef IPC_CREAT
#error "sys/ipc.h:IPC_CREAT macro is missing from libc-shim"
#endif

#ifndef IPC_EXCL
#error "sys/ipc.h:IPC_EXCL macro is missing from libc-shim"
#endif

#ifndef IPC_INFO
#error "sys/ipc.h:IPC_INFO macro is missing from libc-shim"
#endif

#ifndef IPC_NOWAIT
#error "sys/ipc.h:IPC_NOWAIT macro is missing from libc-shim"
#endif

#ifndef IPC_PRIVATE
#error "sys/ipc.h:IPC_PRIVATE macro is missing from libc-shim"
#endif

#ifndef IPC_RMID
#error "sys/ipc.h:IPC_RMID macro is missing from libc-shim"
#endif

#ifndef IPC_SET
#error "sys/ipc.h:IPC_SET macro is missing from libc-shim"
#endif

int main(void) { return 0; }
