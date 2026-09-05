#include <features.h>
#include <stdio.h>

#ifdef _SLATE_LIBC
#if defined(__SLATE_ARCH_X86_64) + defined(__SLATE_ARCH_X86) +                 \
        defined(__SLATE_ARCH_AARCH64) + defined(__SLATE_ARCH_ARM) +            \
        defined(__SLATE_ARCH_RISCV64) + defined(__SLATE_ARCH_RISCV32) !=       \
    1
typedef char invalid_slate_architecture[-1];
#endif
#if defined(__SLATE_VENDOR_UNKNOWN) + defined(__SLATE_VENDOR_PC) +             \
        defined(__SLATE_VENDOR_APPLE) !=                                       \
    1
typedef char invalid_slate_vendor[-1];
#endif
#if defined(__SLATE_KERNEL_LINUX) + defined(__SLATE_KERNEL_WINDOWS) +          \
        defined(__SLATE_KERNEL_DARWIN) !=                                      \
    1
typedef char invalid_slate_kernel[-1];
#endif
#if defined(__SLATE_LIBC_GLIBC) + defined(__SLATE_LIBC_MUSL) +                 \
        defined(__SLATE_LIBC_MINGW) + defined(__SLATE_LIBC_MSVC) +             \
        defined(__SLATE_LIBC_BIONIC) + defined(__SLATE_LIBC_DARWIN) +          \
        defined(__SLATE_LIBC_GENERIC) !=                                       \
    1
typedef char invalid_slate_libc[-1];
#endif

#if defined(EXPECT_MACOS_DARWIN_AARCH64) &&                                    \
    (!defined(__SLATE_ARCH_AARCH64) || !defined(__SLATE_VENDOR_APPLE) ||       \
     !defined(__SLATE_KERNEL_DARWIN) || !defined(__SLATE_PLATFORM_MACOS) ||    \
     !defined(__SLATE_LIBC_DARWIN) || !defined(__SLATE_OBJ_MACHO) ||           \
     !defined(__SLATE_WORDSIZE_64) || !defined(__SLATE_ENDIAN_LITTLE) ||       \
     __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ != 110000)
typedef char invalid_macos_darwin_aarch64_target[-1];
#endif
#if defined(__SLATE_OBJ_ELF) + defined(__SLATE_OBJ_COFF) +                     \
        defined(__SLATE_OBJ_MACHO) !=                                          \
    1
typedef char invalid_slate_object_format[-1];
#endif
#if defined(__SLATE_WORDSIZE_64) + defined(__SLATE_WORDSIZE_32) != 1
typedef char invalid_slate_word_size[-1];
#endif
#if defined(__SLATE_ENDIAN_LITTLE) + defined(__SLATE_ENDIAN_BIG) != 1
typedef char invalid_slate_byte_order[-1];
#endif

#if defined(EXPECT_LINUX_GLIBC_X86_64) &&                                      \
    (!defined(__SLATE_ARCH_X86_64) || !defined(__SLATE_VENDOR_UNKNOWN) ||      \
     !defined(__SLATE_KERNEL_LINUX) || !defined(__SLATE_LIBC_GLIBC) ||         \
     !defined(__SLATE_OBJ_ELF) || !defined(__SLATE_WORDSIZE_64) ||             \
     !defined(__SLATE_ENDIAN_LITTLE))
typedef char invalid_linux_glibc_x86_64_target[-1];
#endif

#if defined(EXPECT_LINUX_MUSL_AARCH64) &&                                      \
    (!defined(__SLATE_ARCH_AARCH64) || !defined(__SLATE_VENDOR_UNKNOWN) ||     \
     !defined(__SLATE_KERNEL_LINUX) || !defined(__SLATE_LIBC_MUSL) ||          \
     !defined(__SLATE_OBJ_ELF) || !defined(__SLATE_WORDSIZE_64) ||             \
     !defined(__SLATE_ENDIAN_LITTLE))
typedef char invalid_linux_musl_aarch64_target[-1];
#endif

#if defined(EXPECT_LINUX_GLIBC_RISCV64) &&                                     \
    (!defined(__SLATE_ARCH_RISCV64) || !defined(__SLATE_VENDOR_UNKNOWN) ||     \
     !defined(__SLATE_KERNEL_LINUX) || !defined(__SLATE_LIBC_GLIBC) ||         \
     !defined(__SLATE_OBJ_ELF) || !defined(__SLATE_WORDSIZE_64) ||             \
     !defined(__SLATE_ENDIAN_LITTLE))
typedef char invalid_linux_glibc_riscv64_target[-1];
#endif

#if defined(EXPECT_WINDOWS_MSVC_X86_64) &&                                     \
    (!defined(__SLATE_ARCH_X86_64) || !defined(__SLATE_VENDOR_PC) ||           \
     !defined(__SLATE_KERNEL_WINDOWS) || !defined(__SLATE_LIBC_MSVC) ||        \
     !defined(__SLATE_OBJ_COFF) || !defined(__SLATE_WORDSIZE_64) ||            \
     !defined(__SLATE_ENDIAN_LITTLE))
typedef char invalid_windows_msvc_x86_64_target[-1];
#endif
#endif

int main(void) {
  printf("target features\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"target features\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"target features\n".as_ptr()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
