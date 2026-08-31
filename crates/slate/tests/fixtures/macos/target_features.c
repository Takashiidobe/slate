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
// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn main() {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn main() {
// REWRITES-MACOS-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-MACOS-NEXT: }
// SLATE-FILECHECK-END rewrites-macos
