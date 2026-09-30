#include <sys/procfs.h>

typedef unsigned long long slate_oracle_typedef_elf_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_elf_greg_t, elf_greg_t), "typedef elf_greg_t differs from oracle");

_Static_assert(sizeof(struct elf_siginfo) == 12, "struct elf_siginfo size differs from oracle");

_Static_assert(_Alignof(struct elf_siginfo) == 4, "struct elf_siginfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct elf_siginfo, si_signo) == 0, "struct elf_siginfo.si_signo offset differs from oracle");

typedef int slate_oracle_struct_elf_siginfo_si_signo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct elf_siginfo *)0)->si_signo), slate_oracle_struct_elf_siginfo_si_signo), "struct elf_siginfo.si_signo field type differs from oracle");

_Static_assert(__builtin_offsetof(struct elf_siginfo, si_code) == 4, "struct elf_siginfo.si_code offset differs from oracle");

typedef int slate_oracle_struct_elf_siginfo_si_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct elf_siginfo *)0)->si_code), slate_oracle_struct_elf_siginfo_si_code), "struct elf_siginfo.si_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct elf_siginfo, si_errno) == 8, "struct elf_siginfo.si_errno offset differs from oracle");

typedef int slate_oracle_struct_elf_siginfo_si_errno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct elf_siginfo *)0)->si_errno), slate_oracle_struct_elf_siginfo_si_errno), "struct elf_siginfo.si_errno field type differs from oracle");

#ifndef ELF_NGREG
#error "sys/procfs.h:ELF_NGREG macro is missing from libc-shim"
#endif

#ifndef ELF_PRARGSZ
#error "sys/procfs.h:ELF_PRARGSZ macro is missing from libc-shim"
#endif

int main(void) { return 0; }
