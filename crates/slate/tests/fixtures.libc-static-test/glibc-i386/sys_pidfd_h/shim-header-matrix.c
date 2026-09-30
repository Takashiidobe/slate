#include <sys/pidfd.h>

_Static_assert(sizeof(struct pidfd_info) == 88, "struct pidfd_info size differs from oracle");

_Static_assert(_Alignof(struct pidfd_info) == 4, "struct pidfd_info alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, mask) == 0, "struct pidfd_info.mask offset differs from oracle");

typedef unsigned long long slate_oracle_struct_pidfd_info_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->mask), slate_oracle_struct_pidfd_info_mask), "struct pidfd_info.mask field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, cgroupid) == 8, "struct pidfd_info.cgroupid offset differs from oracle");

typedef unsigned long long slate_oracle_struct_pidfd_info_cgroupid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->cgroupid), slate_oracle_struct_pidfd_info_cgroupid), "struct pidfd_info.cgroupid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, pid) == 16, "struct pidfd_info.pid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->pid), slate_oracle_struct_pidfd_info_pid), "struct pidfd_info.pid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, tgid) == 20, "struct pidfd_info.tgid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_tgid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->tgid), slate_oracle_struct_pidfd_info_tgid), "struct pidfd_info.tgid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, ppid) == 24, "struct pidfd_info.ppid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_ppid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->ppid), slate_oracle_struct_pidfd_info_ppid), "struct pidfd_info.ppid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, ruid) == 28, "struct pidfd_info.ruid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_ruid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->ruid), slate_oracle_struct_pidfd_info_ruid), "struct pidfd_info.ruid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, rgid) == 32, "struct pidfd_info.rgid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_rgid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->rgid), slate_oracle_struct_pidfd_info_rgid), "struct pidfd_info.rgid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, euid) == 36, "struct pidfd_info.euid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_euid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->euid), slate_oracle_struct_pidfd_info_euid), "struct pidfd_info.euid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, egid) == 40, "struct pidfd_info.egid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_egid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->egid), slate_oracle_struct_pidfd_info_egid), "struct pidfd_info.egid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, suid) == 44, "struct pidfd_info.suid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_suid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->suid), slate_oracle_struct_pidfd_info_suid), "struct pidfd_info.suid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, sgid) == 48, "struct pidfd_info.sgid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_sgid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->sgid), slate_oracle_struct_pidfd_info_sgid), "struct pidfd_info.sgid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, fsuid) == 52, "struct pidfd_info.fsuid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_fsuid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->fsuid), slate_oracle_struct_pidfd_info_fsuid), "struct pidfd_info.fsuid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, fsgid) == 56, "struct pidfd_info.fsgid offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_fsgid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->fsgid), slate_oracle_struct_pidfd_info_fsgid), "struct pidfd_info.fsgid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, exit_code) == 60, "struct pidfd_info.exit_code offset differs from oracle");

typedef int slate_oracle_struct_pidfd_info_exit_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->exit_code), slate_oracle_struct_pidfd_info_exit_code), "struct pidfd_info.exit_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, coredump_mask) == 64, "struct pidfd_info.coredump_mask offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_coredump_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->coredump_mask), slate_oracle_struct_pidfd_info_coredump_mask), "struct pidfd_info.coredump_mask field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, coredump_signal) == 68, "struct pidfd_info.coredump_signal offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_coredump_signal;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->coredump_signal), slate_oracle_struct_pidfd_info_coredump_signal), "struct pidfd_info.coredump_signal field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, coredump_code) == 72, "struct pidfd_info.coredump_code offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_coredump_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->coredump_code), slate_oracle_struct_pidfd_info_coredump_code), "struct pidfd_info.coredump_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, coredump_pad) == 76, "struct pidfd_info.coredump_pad offset differs from oracle");

typedef unsigned int slate_oracle_struct_pidfd_info_coredump_pad;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->coredump_pad), slate_oracle_struct_pidfd_info_coredump_pad), "struct pidfd_info.coredump_pad field type differs from oracle");

_Static_assert(__builtin_offsetof(struct pidfd_info, supported_mask) == 80, "struct pidfd_info.supported_mask offset differs from oracle");

typedef unsigned long long slate_oracle_struct_pidfd_info_supported_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct pidfd_info *)0)->supported_mask), slate_oracle_struct_pidfd_info_supported_mask), "struct pidfd_info.supported_mask field type differs from oracle");

#ifndef PIDFD_COREDUMPED
#error "sys/pidfd.h:PIDFD_COREDUMPED macro is missing from libc-shim"
#endif

#ifndef PIDFD_COREDUMP_ROOT
#error "sys/pidfd.h:PIDFD_COREDUMP_ROOT macro is missing from libc-shim"
#endif

#ifndef PIDFD_COREDUMP_SKIP
#error "sys/pidfd.h:PIDFD_COREDUMP_SKIP macro is missing from libc-shim"
#endif

#ifndef PIDFD_COREDUMP_USER
#error "sys/pidfd.h:PIDFD_COREDUMP_USER macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_CGROUP_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_CGROUP_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_INFO
#error "sys/pidfd.h:PIDFD_GET_INFO macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_IPC_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_IPC_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_MNT_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_MNT_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_NET_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_NET_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_PID_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_PID_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_TIME_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_TIME_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_USER_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_USER_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_UTS_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_UTS_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_CGROUPID
#error "sys/pidfd.h:PIDFD_INFO_CGROUPID macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_COREDUMP
#error "sys/pidfd.h:PIDFD_INFO_COREDUMP macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_COREDUMP_CODE
#error "sys/pidfd.h:PIDFD_INFO_COREDUMP_CODE macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_COREDUMP_SIGNAL
#error "sys/pidfd.h:PIDFD_INFO_COREDUMP_SIGNAL macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_CREDS
#error "sys/pidfd.h:PIDFD_INFO_CREDS macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_EXIT
#error "sys/pidfd.h:PIDFD_INFO_EXIT macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_PID
#error "sys/pidfd.h:PIDFD_INFO_PID macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_SIZE_VER0
#error "sys/pidfd.h:PIDFD_INFO_SIZE_VER0 macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_SIZE_VER1
#error "sys/pidfd.h:PIDFD_INFO_SIZE_VER1 macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_SIZE_VER2
#error "sys/pidfd.h:PIDFD_INFO_SIZE_VER2 macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_SIZE_VER3
#error "sys/pidfd.h:PIDFD_INFO_SIZE_VER3 macro is missing from libc-shim"
#endif

#ifndef PIDFD_INFO_SUPPORTED_MASK
#error "sys/pidfd.h:PIDFD_INFO_SUPPORTED_MASK macro is missing from libc-shim"
#endif

#ifndef PIDFD_NONBLOCK
#error "sys/pidfd.h:PIDFD_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef PIDFD_SELF
#error "sys/pidfd.h:PIDFD_SELF macro is missing from libc-shim"
#endif

#ifndef PIDFD_SELF_PROCESS
#error "sys/pidfd.h:PIDFD_SELF_PROCESS macro is missing from libc-shim"
#endif

#ifndef PIDFD_SELF_THREAD
#error "sys/pidfd.h:PIDFD_SELF_THREAD macro is missing from libc-shim"
#endif

#ifndef PIDFD_SELF_THREAD_GROUP
#error "sys/pidfd.h:PIDFD_SELF_THREAD_GROUP macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_PROCESS_GROUP
#error "sys/pidfd.h:PIDFD_SIGNAL_PROCESS_GROUP macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_THREAD
#error "sys/pidfd.h:PIDFD_SIGNAL_THREAD macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_THREAD_GROUP
#error "sys/pidfd.h:PIDFD_SIGNAL_THREAD_GROUP macro is missing from libc-shim"
#endif

#ifndef PIDFD_THREAD
#error "sys/pidfd.h:PIDFD_THREAD macro is missing from libc-shim"
#endif

#ifndef PIDFS_IOCTL_MAGIC
#error "sys/pidfd.h:PIDFS_IOCTL_MAGIC macro is missing from libc-shim"
#endif

int main(void) { return 0; }
