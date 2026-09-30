#include <sys/prctl.h>

_Static_assert(sizeof(struct prctl_mm_map) == 104, "struct prctl_mm_map size differs from oracle");

_Static_assert(_Alignof(struct prctl_mm_map) == 8, "struct prctl_mm_map alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, start_code) == 0, "struct prctl_mm_map.start_code offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_start_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->start_code), slate_oracle_struct_prctl_mm_map_start_code), "struct prctl_mm_map.start_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, end_code) == 8, "struct prctl_mm_map.end_code offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_end_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->end_code), slate_oracle_struct_prctl_mm_map_end_code), "struct prctl_mm_map.end_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, start_data) == 16, "struct prctl_mm_map.start_data offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_start_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->start_data), slate_oracle_struct_prctl_mm_map_start_data), "struct prctl_mm_map.start_data field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, end_data) == 24, "struct prctl_mm_map.end_data offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_end_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->end_data), slate_oracle_struct_prctl_mm_map_end_data), "struct prctl_mm_map.end_data field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, start_brk) == 32, "struct prctl_mm_map.start_brk offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_start_brk;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->start_brk), slate_oracle_struct_prctl_mm_map_start_brk), "struct prctl_mm_map.start_brk field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, brk) == 40, "struct prctl_mm_map.brk offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_brk;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->brk), slate_oracle_struct_prctl_mm_map_brk), "struct prctl_mm_map.brk field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, start_stack) == 48, "struct prctl_mm_map.start_stack offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_start_stack;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->start_stack), slate_oracle_struct_prctl_mm_map_start_stack), "struct prctl_mm_map.start_stack field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, arg_start) == 56, "struct prctl_mm_map.arg_start offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_arg_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->arg_start), slate_oracle_struct_prctl_mm_map_arg_start), "struct prctl_mm_map.arg_start field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, arg_end) == 64, "struct prctl_mm_map.arg_end offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_arg_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->arg_end), slate_oracle_struct_prctl_mm_map_arg_end), "struct prctl_mm_map.arg_end field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, env_start) == 72, "struct prctl_mm_map.env_start offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_env_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->env_start), slate_oracle_struct_prctl_mm_map_env_start), "struct prctl_mm_map.env_start field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, env_end) == 80, "struct prctl_mm_map.env_end offset differs from oracle");

typedef unsigned long slate_oracle_struct_prctl_mm_map_env_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->env_end), slate_oracle_struct_prctl_mm_map_env_end), "struct prctl_mm_map.env_end field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, auxv) == 88, "struct prctl_mm_map.auxv offset differs from oracle");

typedef unsigned long * slate_oracle_struct_prctl_mm_map_auxv;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->auxv), slate_oracle_struct_prctl_mm_map_auxv), "struct prctl_mm_map.auxv field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, auxv_size) == 96, "struct prctl_mm_map.auxv_size offset differs from oracle");

typedef unsigned int slate_oracle_struct_prctl_mm_map_auxv_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->auxv_size), slate_oracle_struct_prctl_mm_map_auxv_size), "struct prctl_mm_map.auxv_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prctl_mm_map, exe_fd) == 100, "struct prctl_mm_map.exe_fd offset differs from oracle");

typedef unsigned int slate_oracle_struct_prctl_mm_map_exe_fd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prctl_mm_map *)0)->exe_fd), slate_oracle_struct_prctl_mm_map_exe_fd), "struct prctl_mm_map.exe_fd field type differs from oracle");

#ifndef PR_CAPBSET_DROP
#error "sys/prctl.h:PR_CAPBSET_DROP macro is missing from libc-shim"
#endif

#ifndef PR_CAPBSET_READ
#error "sys/prctl.h:PR_CAPBSET_READ macro is missing from libc-shim"
#endif

#ifndef PR_CAP_AMBIENT
#error "sys/prctl.h:PR_CAP_AMBIENT macro is missing from libc-shim"
#endif

#ifndef PR_CAP_AMBIENT_CLEAR_ALL
#error "sys/prctl.h:PR_CAP_AMBIENT_CLEAR_ALL macro is missing from libc-shim"
#endif

#ifndef PR_CAP_AMBIENT_IS_SET
#error "sys/prctl.h:PR_CAP_AMBIENT_IS_SET macro is missing from libc-shim"
#endif

#ifndef PR_CAP_AMBIENT_LOWER
#error "sys/prctl.h:PR_CAP_AMBIENT_LOWER macro is missing from libc-shim"
#endif

#ifndef PR_CAP_AMBIENT_RAISE
#error "sys/prctl.h:PR_CAP_AMBIENT_RAISE macro is missing from libc-shim"
#endif

#ifndef PR_ENDIAN_BIG
#error "sys/prctl.h:PR_ENDIAN_BIG macro is missing from libc-shim"
#endif

#ifndef PR_ENDIAN_LITTLE
#error "sys/prctl.h:PR_ENDIAN_LITTLE macro is missing from libc-shim"
#endif

#ifndef PR_ENDIAN_PPC_LITTLE
#error "sys/prctl.h:PR_ENDIAN_PPC_LITTLE macro is missing from libc-shim"
#endif

#ifndef PR_FPEMU_NOPRINT
#error "sys/prctl.h:PR_FPEMU_NOPRINT macro is missing from libc-shim"
#endif

#ifndef PR_FPEMU_SIGFPE
#error "sys/prctl.h:PR_FPEMU_SIGFPE macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_ASYNC
#error "sys/prctl.h:PR_FP_EXC_ASYNC macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_DISABLED
#error "sys/prctl.h:PR_FP_EXC_DISABLED macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_DIV
#error "sys/prctl.h:PR_FP_EXC_DIV macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_INV
#error "sys/prctl.h:PR_FP_EXC_INV macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_NONRECOV
#error "sys/prctl.h:PR_FP_EXC_NONRECOV macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_OVF
#error "sys/prctl.h:PR_FP_EXC_OVF macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_PRECISE
#error "sys/prctl.h:PR_FP_EXC_PRECISE macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_RES
#error "sys/prctl.h:PR_FP_EXC_RES macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_SW_ENABLE
#error "sys/prctl.h:PR_FP_EXC_SW_ENABLE macro is missing from libc-shim"
#endif

#ifndef PR_FP_EXC_UND
#error "sys/prctl.h:PR_FP_EXC_UND macro is missing from libc-shim"
#endif

#ifndef PR_FP_MODE_FR
#error "sys/prctl.h:PR_FP_MODE_FR macro is missing from libc-shim"
#endif

#ifndef PR_FP_MODE_FRE
#error "sys/prctl.h:PR_FP_MODE_FRE macro is missing from libc-shim"
#endif

#ifndef PR_GET_CHILD_SUBREAPER
#error "sys/prctl.h:PR_GET_CHILD_SUBREAPER macro is missing from libc-shim"
#endif

#ifndef PR_GET_DUMPABLE
#error "sys/prctl.h:PR_GET_DUMPABLE macro is missing from libc-shim"
#endif

#ifndef PR_GET_ENDIAN
#error "sys/prctl.h:PR_GET_ENDIAN macro is missing from libc-shim"
#endif

#ifndef PR_GET_FPEMU
#error "sys/prctl.h:PR_GET_FPEMU macro is missing from libc-shim"
#endif

#ifndef PR_GET_FPEXC
#error "sys/prctl.h:PR_GET_FPEXC macro is missing from libc-shim"
#endif

#ifndef PR_GET_FP_MODE
#error "sys/prctl.h:PR_GET_FP_MODE macro is missing from libc-shim"
#endif

#ifndef PR_GET_IO_FLUSHER
#error "sys/prctl.h:PR_GET_IO_FLUSHER macro is missing from libc-shim"
#endif

#ifndef PR_GET_KEEPCAPS
#error "sys/prctl.h:PR_GET_KEEPCAPS macro is missing from libc-shim"
#endif

#ifndef PR_GET_NAME
#error "sys/prctl.h:PR_GET_NAME macro is missing from libc-shim"
#endif

#ifndef PR_GET_NO_NEW_PRIVS
#error "sys/prctl.h:PR_GET_NO_NEW_PRIVS macro is missing from libc-shim"
#endif

#ifndef PR_GET_PDEATHSIG
#error "sys/prctl.h:PR_GET_PDEATHSIG macro is missing from libc-shim"
#endif

#ifndef PR_GET_SECCOMP
#error "sys/prctl.h:PR_GET_SECCOMP macro is missing from libc-shim"
#endif

#ifndef PR_GET_SECUREBITS
#error "sys/prctl.h:PR_GET_SECUREBITS macro is missing from libc-shim"
#endif

#ifndef PR_GET_SPECULATION_CTRL
#error "sys/prctl.h:PR_GET_SPECULATION_CTRL macro is missing from libc-shim"
#endif

#ifndef PR_GET_TAGGED_ADDR_CTRL
#error "sys/prctl.h:PR_GET_TAGGED_ADDR_CTRL macro is missing from libc-shim"
#endif

#ifndef PR_GET_THP_DISABLE
#error "sys/prctl.h:PR_GET_THP_DISABLE macro is missing from libc-shim"
#endif

#ifndef PR_GET_TID_ADDRESS
#error "sys/prctl.h:PR_GET_TID_ADDRESS macro is missing from libc-shim"
#endif

#ifndef PR_GET_TIMERSLACK
#error "sys/prctl.h:PR_GET_TIMERSLACK macro is missing from libc-shim"
#endif

#ifndef PR_GET_TIMING
#error "sys/prctl.h:PR_GET_TIMING macro is missing from libc-shim"
#endif

#ifndef PR_GET_TSC
#error "sys/prctl.h:PR_GET_TSC macro is missing from libc-shim"
#endif

#ifndef PR_GET_UNALIGN
#error "sys/prctl.h:PR_GET_UNALIGN macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL
#error "sys/prctl.h:PR_MCE_KILL macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_CLEAR
#error "sys/prctl.h:PR_MCE_KILL_CLEAR macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_DEFAULT
#error "sys/prctl.h:PR_MCE_KILL_DEFAULT macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_EARLY
#error "sys/prctl.h:PR_MCE_KILL_EARLY macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_GET
#error "sys/prctl.h:PR_MCE_KILL_GET macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_LATE
#error "sys/prctl.h:PR_MCE_KILL_LATE macro is missing from libc-shim"
#endif

#ifndef PR_MCE_KILL_SET
#error "sys/prctl.h:PR_MCE_KILL_SET macro is missing from libc-shim"
#endif

#ifndef PR_MPX_DISABLE_MANAGEMENT
#error "sys/prctl.h:PR_MPX_DISABLE_MANAGEMENT macro is missing from libc-shim"
#endif

#ifndef PR_MPX_ENABLE_MANAGEMENT
#error "sys/prctl.h:PR_MPX_ENABLE_MANAGEMENT macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TAG_MASK
#error "sys/prctl.h:PR_MTE_TAG_MASK macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TAG_SHIFT
#error "sys/prctl.h:PR_MTE_TAG_SHIFT macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TCF_ASYNC
#error "sys/prctl.h:PR_MTE_TCF_ASYNC macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TCF_MASK
#error "sys/prctl.h:PR_MTE_TCF_MASK macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TCF_NONE
#error "sys/prctl.h:PR_MTE_TCF_NONE macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TCF_SHIFT
#error "sys/prctl.h:PR_MTE_TCF_SHIFT macro is missing from libc-shim"
#endif

#ifndef PR_MTE_TCF_SYNC
#error "sys/prctl.h:PR_MTE_TCF_SYNC macro is missing from libc-shim"
#endif

#ifndef PR_PAC_APDAKEY
#error "sys/prctl.h:PR_PAC_APDAKEY macro is missing from libc-shim"
#endif

#ifndef PR_PAC_APDBKEY
#error "sys/prctl.h:PR_PAC_APDBKEY macro is missing from libc-shim"
#endif

#ifndef PR_PAC_APGAKEY
#error "sys/prctl.h:PR_PAC_APGAKEY macro is missing from libc-shim"
#endif

#ifndef PR_PAC_APIAKEY
#error "sys/prctl.h:PR_PAC_APIAKEY macro is missing from libc-shim"
#endif

#ifndef PR_PAC_APIBKEY
#error "sys/prctl.h:PR_PAC_APIBKEY macro is missing from libc-shim"
#endif

#ifndef PR_PAC_GET_ENABLED_KEYS
#error "sys/prctl.h:PR_PAC_GET_ENABLED_KEYS macro is missing from libc-shim"
#endif

#ifndef PR_PAC_RESET_KEYS
#error "sys/prctl.h:PR_PAC_RESET_KEYS macro is missing from libc-shim"
#endif

#ifndef PR_PAC_SET_ENABLED_KEYS
#error "sys/prctl.h:PR_PAC_SET_ENABLED_KEYS macro is missing from libc-shim"
#endif

#ifndef PR_SET_CHILD_SUBREAPER
#error "sys/prctl.h:PR_SET_CHILD_SUBREAPER macro is missing from libc-shim"
#endif

#ifndef PR_SET_DUMPABLE
#error "sys/prctl.h:PR_SET_DUMPABLE macro is missing from libc-shim"
#endif

#ifndef PR_SET_ENDIAN
#error "sys/prctl.h:PR_SET_ENDIAN macro is missing from libc-shim"
#endif

#ifndef PR_SET_FPEMU
#error "sys/prctl.h:PR_SET_FPEMU macro is missing from libc-shim"
#endif

#ifndef PR_SET_FPEXC
#error "sys/prctl.h:PR_SET_FPEXC macro is missing from libc-shim"
#endif

#ifndef PR_SET_FP_MODE
#error "sys/prctl.h:PR_SET_FP_MODE macro is missing from libc-shim"
#endif

#ifndef PR_SET_IO_FLUSHER
#error "sys/prctl.h:PR_SET_IO_FLUSHER macro is missing from libc-shim"
#endif

#ifndef PR_SET_KEEPCAPS
#error "sys/prctl.h:PR_SET_KEEPCAPS macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM
#error "sys/prctl.h:PR_SET_MM macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_ARG_END
#error "sys/prctl.h:PR_SET_MM_ARG_END macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_ARG_START
#error "sys/prctl.h:PR_SET_MM_ARG_START macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_AUXV
#error "sys/prctl.h:PR_SET_MM_AUXV macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_BRK
#error "sys/prctl.h:PR_SET_MM_BRK macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_END_CODE
#error "sys/prctl.h:PR_SET_MM_END_CODE macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_END_DATA
#error "sys/prctl.h:PR_SET_MM_END_DATA macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_ENV_END
#error "sys/prctl.h:PR_SET_MM_ENV_END macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_ENV_START
#error "sys/prctl.h:PR_SET_MM_ENV_START macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_EXE_FILE
#error "sys/prctl.h:PR_SET_MM_EXE_FILE macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_MAP
#error "sys/prctl.h:PR_SET_MM_MAP macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_MAP_SIZE
#error "sys/prctl.h:PR_SET_MM_MAP_SIZE macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_START_BRK
#error "sys/prctl.h:PR_SET_MM_START_BRK macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_START_CODE
#error "sys/prctl.h:PR_SET_MM_START_CODE macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_START_DATA
#error "sys/prctl.h:PR_SET_MM_START_DATA macro is missing from libc-shim"
#endif

#ifndef PR_SET_MM_START_STACK
#error "sys/prctl.h:PR_SET_MM_START_STACK macro is missing from libc-shim"
#endif

#ifndef PR_SET_NAME
#error "sys/prctl.h:PR_SET_NAME macro is missing from libc-shim"
#endif

#ifndef PR_SET_NO_NEW_PRIVS
#error "sys/prctl.h:PR_SET_NO_NEW_PRIVS macro is missing from libc-shim"
#endif

#ifndef PR_SET_PDEATHSIG
#error "sys/prctl.h:PR_SET_PDEATHSIG macro is missing from libc-shim"
#endif

#ifndef PR_SET_PTRACER
#error "sys/prctl.h:PR_SET_PTRACER macro is missing from libc-shim"
#endif

#ifndef PR_SET_PTRACER_ANY
#error "sys/prctl.h:PR_SET_PTRACER_ANY macro is missing from libc-shim"
#endif

#ifndef PR_SET_SECCOMP
#error "sys/prctl.h:PR_SET_SECCOMP macro is missing from libc-shim"
#endif

#ifndef PR_SET_SECUREBITS
#error "sys/prctl.h:PR_SET_SECUREBITS macro is missing from libc-shim"
#endif

#ifndef PR_SET_SPECULATION_CTRL
#error "sys/prctl.h:PR_SET_SPECULATION_CTRL macro is missing from libc-shim"
#endif

#ifndef PR_SET_SYSCALL_USER_DISPATCH
#error "sys/prctl.h:PR_SET_SYSCALL_USER_DISPATCH macro is missing from libc-shim"
#endif

#ifndef PR_SET_TAGGED_ADDR_CTRL
#error "sys/prctl.h:PR_SET_TAGGED_ADDR_CTRL macro is missing from libc-shim"
#endif

#ifndef PR_SET_THP_DISABLE
#error "sys/prctl.h:PR_SET_THP_DISABLE macro is missing from libc-shim"
#endif

#ifndef PR_SET_TIMERSLACK
#error "sys/prctl.h:PR_SET_TIMERSLACK macro is missing from libc-shim"
#endif

#ifndef PR_SET_TIMING
#error "sys/prctl.h:PR_SET_TIMING macro is missing from libc-shim"
#endif

#ifndef PR_SET_TSC
#error "sys/prctl.h:PR_SET_TSC macro is missing from libc-shim"
#endif

#ifndef PR_SET_UNALIGN
#error "sys/prctl.h:PR_SET_UNALIGN macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_DISABLE
#error "sys/prctl.h:PR_SPEC_DISABLE macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_DISABLE_NOEXEC
#error "sys/prctl.h:PR_SPEC_DISABLE_NOEXEC macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_ENABLE
#error "sys/prctl.h:PR_SPEC_ENABLE macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_FORCE_DISABLE
#error "sys/prctl.h:PR_SPEC_FORCE_DISABLE macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_INDIRECT_BRANCH
#error "sys/prctl.h:PR_SPEC_INDIRECT_BRANCH macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_NOT_AFFECTED
#error "sys/prctl.h:PR_SPEC_NOT_AFFECTED macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_PRCTL
#error "sys/prctl.h:PR_SPEC_PRCTL macro is missing from libc-shim"
#endif

#ifndef PR_SPEC_STORE_BYPASS
#error "sys/prctl.h:PR_SPEC_STORE_BYPASS macro is missing from libc-shim"
#endif

#ifndef PR_SVE_GET_VL
#error "sys/prctl.h:PR_SVE_GET_VL macro is missing from libc-shim"
#endif

#ifndef PR_SVE_SET_VL
#error "sys/prctl.h:PR_SVE_SET_VL macro is missing from libc-shim"
#endif

#ifndef PR_SVE_SET_VL_ONEXEC
#error "sys/prctl.h:PR_SVE_SET_VL_ONEXEC macro is missing from libc-shim"
#endif

#ifndef PR_SVE_VL_INHERIT
#error "sys/prctl.h:PR_SVE_VL_INHERIT macro is missing from libc-shim"
#endif

#ifndef PR_SVE_VL_LEN_MASK
#error "sys/prctl.h:PR_SVE_VL_LEN_MASK macro is missing from libc-shim"
#endif

#ifndef PR_SYS_DISPATCH_OFF
#error "sys/prctl.h:PR_SYS_DISPATCH_OFF macro is missing from libc-shim"
#endif

#ifndef PR_SYS_DISPATCH_ON
#error "sys/prctl.h:PR_SYS_DISPATCH_ON macro is missing from libc-shim"
#endif

#ifndef PR_TAGGED_ADDR_ENABLE
#error "sys/prctl.h:PR_TAGGED_ADDR_ENABLE macro is missing from libc-shim"
#endif

#ifndef PR_TASK_PERF_EVENTS_DISABLE
#error "sys/prctl.h:PR_TASK_PERF_EVENTS_DISABLE macro is missing from libc-shim"
#endif

#ifndef PR_TASK_PERF_EVENTS_ENABLE
#error "sys/prctl.h:PR_TASK_PERF_EVENTS_ENABLE macro is missing from libc-shim"
#endif

#ifndef PR_TIMING_STATISTICAL
#error "sys/prctl.h:PR_TIMING_STATISTICAL macro is missing from libc-shim"
#endif

#ifndef PR_TIMING_TIMESTAMP
#error "sys/prctl.h:PR_TIMING_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef PR_TSC_ENABLE
#error "sys/prctl.h:PR_TSC_ENABLE macro is missing from libc-shim"
#endif

#ifndef PR_TSC_SIGSEGV
#error "sys/prctl.h:PR_TSC_SIGSEGV macro is missing from libc-shim"
#endif

#ifndef PR_UNALIGN_NOPRINT
#error "sys/prctl.h:PR_UNALIGN_NOPRINT macro is missing from libc-shim"
#endif

#ifndef PR_UNALIGN_SIGBUS
#error "sys/prctl.h:PR_UNALIGN_SIGBUS macro is missing from libc-shim"
#endif

#ifndef SYSCALL_DISPATCH_FILTER_ALLOW
#error "sys/prctl.h:SYSCALL_DISPATCH_FILTER_ALLOW macro is missing from libc-shim"
#endif

#ifndef SYSCALL_DISPATCH_FILTER_BLOCK
#error "sys/prctl.h:SYSCALL_DISPATCH_FILTER_BLOCK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
