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




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 97, 114, 103, 101, 116, 32, 102, 101, 97, 116, 117, 114, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
