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



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       variadic: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               7,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   7,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLit(
// DEFAULT-NEXT:                               "target features\\n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       0,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 81,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
