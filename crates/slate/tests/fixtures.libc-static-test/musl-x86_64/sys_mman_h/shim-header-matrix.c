#include <sys/mman.h>

extern void * slate_oracle_mmap(void *, unsigned long, int, int, int, long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mmap), __typeof__(mmap)),
    "sys/mman.h:mmap declaration differs from oracle");

static __typeof__(mmap) *const slate_reference_mmap = &mmap;

#ifndef MADV_COLD
#error "sys/mman.h:MADV_COLD macro is missing from libc-shim"
#endif

#ifndef MADV_COLLAPSE
#error "sys/mman.h:MADV_COLLAPSE macro is missing from libc-shim"
#endif

#ifndef MADV_DODUMP
#error "sys/mman.h:MADV_DODUMP macro is missing from libc-shim"
#endif

#ifndef MADV_DOFORK
#error "sys/mman.h:MADV_DOFORK macro is missing from libc-shim"
#endif

#ifndef MADV_DONTDUMP
#error "sys/mman.h:MADV_DONTDUMP macro is missing from libc-shim"
#endif

#ifndef MADV_DONTFORK
#error "sys/mman.h:MADV_DONTFORK macro is missing from libc-shim"
#endif

#ifndef MADV_DONTNEED
#error "sys/mman.h:MADV_DONTNEED macro is missing from libc-shim"
#endif

#ifndef MADV_DONTNEED_LOCKED
#error "sys/mman.h:MADV_DONTNEED_LOCKED macro is missing from libc-shim"
#endif

#ifndef MADV_FREE
#error "sys/mman.h:MADV_FREE macro is missing from libc-shim"
#endif

#ifndef MADV_HUGEPAGE
#error "sys/mman.h:MADV_HUGEPAGE macro is missing from libc-shim"
#endif

#ifndef MADV_HWPOISON
#error "sys/mman.h:MADV_HWPOISON macro is missing from libc-shim"
#endif

#ifndef MADV_KEEPONFORK
#error "sys/mman.h:MADV_KEEPONFORK macro is missing from libc-shim"
#endif

#ifndef MADV_MERGEABLE
#error "sys/mman.h:MADV_MERGEABLE macro is missing from libc-shim"
#endif

#ifndef MADV_NOHUGEPAGE
#error "sys/mman.h:MADV_NOHUGEPAGE macro is missing from libc-shim"
#endif

#ifndef MADV_NORMAL
#error "sys/mman.h:MADV_NORMAL macro is missing from libc-shim"
#endif

#ifndef MADV_PAGEOUT
#error "sys/mman.h:MADV_PAGEOUT macro is missing from libc-shim"
#endif

#ifndef MADV_POPULATE_READ
#error "sys/mman.h:MADV_POPULATE_READ macro is missing from libc-shim"
#endif

#ifndef MADV_POPULATE_WRITE
#error "sys/mman.h:MADV_POPULATE_WRITE macro is missing from libc-shim"
#endif

#ifndef MADV_RANDOM
#error "sys/mman.h:MADV_RANDOM macro is missing from libc-shim"
#endif

#ifndef MADV_REMOVE
#error "sys/mman.h:MADV_REMOVE macro is missing from libc-shim"
#endif

#ifndef MADV_SEQUENTIAL
#error "sys/mman.h:MADV_SEQUENTIAL macro is missing from libc-shim"
#endif

#ifndef MADV_SOFT_OFFLINE
#error "sys/mman.h:MADV_SOFT_OFFLINE macro is missing from libc-shim"
#endif

#ifndef MADV_UNMERGEABLE
#error "sys/mman.h:MADV_UNMERGEABLE macro is missing from libc-shim"
#endif

#ifndef MADV_WILLNEED
#error "sys/mman.h:MADV_WILLNEED macro is missing from libc-shim"
#endif

#ifndef MADV_WIPEONFORK
#error "sys/mman.h:MADV_WIPEONFORK macro is missing from libc-shim"
#endif

#ifndef MAP_32BIT
#error "sys/mman.h:MAP_32BIT macro is missing from libc-shim"
#endif

#ifndef MAP_ANON
#error "sys/mman.h:MAP_ANON macro is missing from libc-shim"
#endif

#ifndef MAP_ANONYMOUS
#error "sys/mman.h:MAP_ANONYMOUS macro is missing from libc-shim"
#endif

#ifndef MAP_DENYWRITE
#error "sys/mman.h:MAP_DENYWRITE macro is missing from libc-shim"
#endif

#ifndef MAP_EXECUTABLE
#error "sys/mman.h:MAP_EXECUTABLE macro is missing from libc-shim"
#endif

#ifndef MAP_FAILED
#error "sys/mman.h:MAP_FAILED macro is missing from libc-shim"
#endif

#ifndef MAP_FILE
#error "sys/mman.h:MAP_FILE macro is missing from libc-shim"
#endif

#ifndef MAP_FIXED
#error "sys/mman.h:MAP_FIXED macro is missing from libc-shim"
#endif

#ifndef MAP_FIXED_NOREPLACE
#error "sys/mman.h:MAP_FIXED_NOREPLACE macro is missing from libc-shim"
#endif

#ifndef MAP_GROWSDOWN
#error "sys/mman.h:MAP_GROWSDOWN macro is missing from libc-shim"
#endif

#ifndef MAP_HUGETLB
#error "sys/mman.h:MAP_HUGETLB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_16GB
#error "sys/mman.h:MAP_HUGE_16GB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_16KB
#error "sys/mman.h:MAP_HUGE_16KB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_16MB
#error "sys/mman.h:MAP_HUGE_16MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_1GB
#error "sys/mman.h:MAP_HUGE_1GB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_1MB
#error "sys/mman.h:MAP_HUGE_1MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_256MB
#error "sys/mman.h:MAP_HUGE_256MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_2GB
#error "sys/mman.h:MAP_HUGE_2GB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_2MB
#error "sys/mman.h:MAP_HUGE_2MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_32MB
#error "sys/mman.h:MAP_HUGE_32MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_512KB
#error "sys/mman.h:MAP_HUGE_512KB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_512MB
#error "sys/mman.h:MAP_HUGE_512MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_64KB
#error "sys/mman.h:MAP_HUGE_64KB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_8MB
#error "sys/mman.h:MAP_HUGE_8MB macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_MASK
#error "sys/mman.h:MAP_HUGE_MASK macro is missing from libc-shim"
#endif

#ifndef MAP_HUGE_SHIFT
#error "sys/mman.h:MAP_HUGE_SHIFT macro is missing from libc-shim"
#endif

#ifndef MAP_LOCKED
#error "sys/mman.h:MAP_LOCKED macro is missing from libc-shim"
#endif

#ifndef MAP_NONBLOCK
#error "sys/mman.h:MAP_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef MAP_NORESERVE
#error "sys/mman.h:MAP_NORESERVE macro is missing from libc-shim"
#endif

#ifndef MAP_POPULATE
#error "sys/mman.h:MAP_POPULATE macro is missing from libc-shim"
#endif

#ifndef MAP_PRIVATE
#error "sys/mman.h:MAP_PRIVATE macro is missing from libc-shim"
#endif

#ifndef MAP_SHARED
#error "sys/mman.h:MAP_SHARED macro is missing from libc-shim"
#endif

#ifndef MAP_SHARED_VALIDATE
#error "sys/mman.h:MAP_SHARED_VALIDATE macro is missing from libc-shim"
#endif

#ifndef MAP_STACK
#error "sys/mman.h:MAP_STACK macro is missing from libc-shim"
#endif

#ifndef MAP_SYNC
#error "sys/mman.h:MAP_SYNC macro is missing from libc-shim"
#endif

#ifndef MAP_TYPE
#error "sys/mman.h:MAP_TYPE macro is missing from libc-shim"
#endif

#ifndef MCL_CURRENT
#error "sys/mman.h:MCL_CURRENT macro is missing from libc-shim"
#endif

#ifndef MCL_FUTURE
#error "sys/mman.h:MCL_FUTURE macro is missing from libc-shim"
#endif

#ifndef MCL_ONFAULT
#error "sys/mman.h:MCL_ONFAULT macro is missing from libc-shim"
#endif

#ifndef MFD_ALLOW_SEALING
#error "sys/mman.h:MFD_ALLOW_SEALING macro is missing from libc-shim"
#endif

#ifndef MFD_CLOEXEC
#error "sys/mman.h:MFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef MFD_HUGETLB
#error "sys/mman.h:MFD_HUGETLB macro is missing from libc-shim"
#endif

#ifndef MLOCK_ONFAULT
#error "sys/mman.h:MLOCK_ONFAULT macro is missing from libc-shim"
#endif

#ifndef MREMAP_DONTUNMAP
#error "sys/mman.h:MREMAP_DONTUNMAP macro is missing from libc-shim"
#endif

#ifndef MREMAP_FIXED
#error "sys/mman.h:MREMAP_FIXED macro is missing from libc-shim"
#endif

#ifndef MREMAP_MAYMOVE
#error "sys/mman.h:MREMAP_MAYMOVE macro is missing from libc-shim"
#endif

#ifndef MS_ASYNC
#error "sys/mman.h:MS_ASYNC macro is missing from libc-shim"
#endif

#ifndef MS_INVALIDATE
#error "sys/mman.h:MS_INVALIDATE macro is missing from libc-shim"
#endif

#ifndef MS_SYNC
#error "sys/mman.h:MS_SYNC macro is missing from libc-shim"
#endif

#ifndef POSIX_MADV_DONTNEED
#error "sys/mman.h:POSIX_MADV_DONTNEED macro is missing from libc-shim"
#endif

#ifndef POSIX_MADV_NORMAL
#error "sys/mman.h:POSIX_MADV_NORMAL macro is missing from libc-shim"
#endif

#ifndef POSIX_MADV_RANDOM
#error "sys/mman.h:POSIX_MADV_RANDOM macro is missing from libc-shim"
#endif

#ifndef POSIX_MADV_SEQUENTIAL
#error "sys/mman.h:POSIX_MADV_SEQUENTIAL macro is missing from libc-shim"
#endif

#ifndef POSIX_MADV_WILLNEED
#error "sys/mman.h:POSIX_MADV_WILLNEED macro is missing from libc-shim"
#endif

#ifndef PROT_EXEC
#error "sys/mman.h:PROT_EXEC macro is missing from libc-shim"
#endif

#ifndef PROT_GROWSDOWN
#error "sys/mman.h:PROT_GROWSDOWN macro is missing from libc-shim"
#endif

#ifndef PROT_GROWSUP
#error "sys/mman.h:PROT_GROWSUP macro is missing from libc-shim"
#endif

#ifndef PROT_NONE
#error "sys/mman.h:PROT_NONE macro is missing from libc-shim"
#endif

#ifndef PROT_READ
#error "sys/mman.h:PROT_READ macro is missing from libc-shim"
#endif

#ifndef PROT_WRITE
#error "sys/mman.h:PROT_WRITE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
