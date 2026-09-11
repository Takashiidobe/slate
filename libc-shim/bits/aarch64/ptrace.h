#if !defined(_SLATE_LIBC)
#error "Never include <bits/aarch64/ptrace.h> directly; include a public header instead."
#endif

#if defined(__SLATE_LIBC_GLIBC)
enum {
  PTRACE_SYSEMU            = 31,
  PTRACE_SYSEMU_SINGLESTEP = 32,
  PTRACE_PEEKMTETAGS       = 33,
  PTRACE_POKEMTETAGS       = 34,
};
#endif
