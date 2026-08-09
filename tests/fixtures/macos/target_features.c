#include <features.h>

#if !defined(__SLATE_ARCH_AARCH64)
#error "AArch64 profile required"
#endif

#if !defined(__SLATE_VENDOR_APPLE)
#error "Apple vendor profile required"
#endif

#if !defined(__SLATE_KERNEL_DARWIN)
#error "Darwin kernel profile required"
#endif

#if !defined(__SLATE_PLATFORM_MACOS)
#error "macOS platform profile required"
#endif

#if !defined(__SLATE_LIBC_DARWIN)
#error "Darwin libc profile required"
#endif

#if !defined(__SLATE_OBJ_MACHO)
#error "Mach-O profile required"
#endif

#if !defined(__SLATE_WORDSIZE_64)
#error "LP64 profile required"
#endif

#if !defined(__SLATE_ENDIAN_LITTLE)
#error "little-endian profile required"
#endif

_Static_assert(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 110000,
               "macOS 11 deployment target required");

int main(void) { return 0; }
