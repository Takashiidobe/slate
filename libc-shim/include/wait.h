#if defined(__SLATE_LIBC_GLIBC)
#include <unistd.h>
#define WCOREFLAG __WCOREFLAG
#define W_EXITCODE(ret, sig) __W_EXITCODE (ret, sig)
#define W_STOPCODE(sig) __W_STOPCODE (sig)
#define WAIT_ANY (-1)
#define WAIT_MYPGRP 0
#if defined(__SLATE_ARCH_AARCH64)
#include <sys/types.h>
#endif
#elif defined(__SLATE_LIBC_MUSL)
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/time.h>
#endif

#include <sys/wait.h>
