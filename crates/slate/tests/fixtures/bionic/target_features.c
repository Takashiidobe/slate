#include <features.h>

#if !defined(__SLATE_LIBC_BIONIC)
#error "Bionic profile required"
#endif

#if !defined(__SLATE_KERNEL_LINUX)
#error "Linux kernel profile required"
#endif

#if !defined(__SLATE_PLATFORM_ANDROID)
#error "Android platform profile required"
#endif

#if !defined(__SLATE_OBJ_ELF)
#error "ELF profile required"
#endif

#if !defined(__SLATE_WORDSIZE_64)
#error "LP64 profile required"
#endif

#if !defined(__SLATE_ENDIAN_LITTLE)
#error "little-endian profile required"
#endif

_Static_assert(__SLATE_ANDROID_API__ == 21, "Android API 21 required");

#if defined(EXPECT_AARCH64) && !defined(__SLATE_ARCH_AARCH64)
#error "AArch64 profile required"
#endif

#if defined(EXPECT_X86_64) && !defined(__SLATE_ARCH_X86_64)
#error "x86-64 profile required"
#endif

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() -> std::process::ExitCode {
// COMMON-REWRITES-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: fn main() -> std::process::ExitCode {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     return std::process::ExitCode::SUCCESS;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
