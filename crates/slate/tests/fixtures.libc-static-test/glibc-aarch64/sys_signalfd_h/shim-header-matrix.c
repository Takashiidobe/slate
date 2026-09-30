#include <sys/signalfd.h>

_Static_assert(sizeof(struct signalfd_siginfo) == 128, "struct signalfd_siginfo size differs from oracle");

_Static_assert(_Alignof(struct signalfd_siginfo) == 8, "struct signalfd_siginfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_signo) == 0, "struct signalfd_siginfo.ssi_signo offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_signo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_signo), slate_oracle_struct_signalfd_siginfo_ssi_signo), "struct signalfd_siginfo.ssi_signo field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_errno) == 4, "struct signalfd_siginfo.ssi_errno offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_errno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_errno), slate_oracle_struct_signalfd_siginfo_ssi_errno), "struct signalfd_siginfo.ssi_errno field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_code) == 8, "struct signalfd_siginfo.ssi_code offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_code), slate_oracle_struct_signalfd_siginfo_ssi_code), "struct signalfd_siginfo.ssi_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_pid) == 12, "struct signalfd_siginfo.ssi_pid offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_pid), slate_oracle_struct_signalfd_siginfo_ssi_pid), "struct signalfd_siginfo.ssi_pid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_uid) == 16, "struct signalfd_siginfo.ssi_uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_uid), slate_oracle_struct_signalfd_siginfo_ssi_uid), "struct signalfd_siginfo.ssi_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_fd) == 20, "struct signalfd_siginfo.ssi_fd offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_fd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_fd), slate_oracle_struct_signalfd_siginfo_ssi_fd), "struct signalfd_siginfo.ssi_fd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_tid) == 24, "struct signalfd_siginfo.ssi_tid offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_tid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_tid), slate_oracle_struct_signalfd_siginfo_ssi_tid), "struct signalfd_siginfo.ssi_tid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_band) == 28, "struct signalfd_siginfo.ssi_band offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_band;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_band), slate_oracle_struct_signalfd_siginfo_ssi_band), "struct signalfd_siginfo.ssi_band field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_overrun) == 32, "struct signalfd_siginfo.ssi_overrun offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_overrun;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_overrun), slate_oracle_struct_signalfd_siginfo_ssi_overrun), "struct signalfd_siginfo.ssi_overrun field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_trapno) == 36, "struct signalfd_siginfo.ssi_trapno offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_trapno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_trapno), slate_oracle_struct_signalfd_siginfo_ssi_trapno), "struct signalfd_siginfo.ssi_trapno field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_status) == 40, "struct signalfd_siginfo.ssi_status offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_status;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_status), slate_oracle_struct_signalfd_siginfo_ssi_status), "struct signalfd_siginfo.ssi_status field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_int) == 44, "struct signalfd_siginfo.ssi_int offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_int;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_int), slate_oracle_struct_signalfd_siginfo_ssi_int), "struct signalfd_siginfo.ssi_int field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_ptr) == 48, "struct signalfd_siginfo.ssi_ptr offset differs from oracle");

typedef unsigned long slate_oracle_struct_signalfd_siginfo_ssi_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_ptr), slate_oracle_struct_signalfd_siginfo_ssi_ptr), "struct signalfd_siginfo.ssi_ptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_utime) == 56, "struct signalfd_siginfo.ssi_utime offset differs from oracle");

typedef unsigned long slate_oracle_struct_signalfd_siginfo_ssi_utime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_utime), slate_oracle_struct_signalfd_siginfo_ssi_utime), "struct signalfd_siginfo.ssi_utime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_stime) == 64, "struct signalfd_siginfo.ssi_stime offset differs from oracle");

typedef unsigned long slate_oracle_struct_signalfd_siginfo_ssi_stime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_stime), slate_oracle_struct_signalfd_siginfo_ssi_stime), "struct signalfd_siginfo.ssi_stime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_addr) == 72, "struct signalfd_siginfo.ssi_addr offset differs from oracle");

typedef unsigned long slate_oracle_struct_signalfd_siginfo_ssi_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_addr), slate_oracle_struct_signalfd_siginfo_ssi_addr), "struct signalfd_siginfo.ssi_addr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_addr_lsb) == 80, "struct signalfd_siginfo.ssi_addr_lsb offset differs from oracle");

typedef unsigned short slate_oracle_struct_signalfd_siginfo_ssi_addr_lsb;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_addr_lsb), slate_oracle_struct_signalfd_siginfo_ssi_addr_lsb), "struct signalfd_siginfo.ssi_addr_lsb field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, __pad2) == 82, "struct signalfd_siginfo.__pad2 offset differs from oracle");

typedef unsigned short slate_oracle_struct_signalfd_siginfo___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->__pad2), slate_oracle_struct_signalfd_siginfo___pad2), "struct signalfd_siginfo.__pad2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_syscall) == 84, "struct signalfd_siginfo.ssi_syscall offset differs from oracle");

typedef int slate_oracle_struct_signalfd_siginfo_ssi_syscall;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_syscall), slate_oracle_struct_signalfd_siginfo_ssi_syscall), "struct signalfd_siginfo.ssi_syscall field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_call_addr) == 88, "struct signalfd_siginfo.ssi_call_addr offset differs from oracle");

typedef unsigned long slate_oracle_struct_signalfd_siginfo_ssi_call_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_call_addr), slate_oracle_struct_signalfd_siginfo_ssi_call_addr), "struct signalfd_siginfo.ssi_call_addr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, ssi_arch) == 96, "struct signalfd_siginfo.ssi_arch offset differs from oracle");

typedef unsigned int slate_oracle_struct_signalfd_siginfo_ssi_arch;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct signalfd_siginfo *)0)->ssi_arch), slate_oracle_struct_signalfd_siginfo_ssi_arch), "struct signalfd_siginfo.ssi_arch field type differs from oracle");

_Static_assert(__builtin_offsetof(struct signalfd_siginfo, __pad) == 100, "struct signalfd_siginfo.__pad offset differs from oracle");

#ifndef SFD_CLOEXEC
#error "sys/signalfd.h:SFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef SFD_NONBLOCK
#error "sys/signalfd.h:SFD_NONBLOCK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
