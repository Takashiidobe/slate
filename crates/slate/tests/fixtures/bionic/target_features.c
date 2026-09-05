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

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64: #![allow(
// LOWERING-BIONIC-AARCH64-NEXT:     dead_code,
// LOWERING-BIONIC-AARCH64-NEXT:     unused,
// LOWERING-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-AARCH64-NEXT:     non_snake_case,
// LOWERING-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-AARCH64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-AARCH64-NEXT:     unused_comparisons
// LOWERING-BIONIC-AARCH64-NEXT: )]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64: #![allow(
// LOWERING-BIONIC-X86_64-NEXT:     dead_code,
// LOWERING-BIONIC-X86_64-NEXT:     unused,
// LOWERING-BIONIC-X86_64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-X86_64-NEXT:     non_snake_case,
// LOWERING-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-X86_64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-X86_64-NEXT:     unused_comparisons
// LOWERING-BIONIC-X86_64-NEXT: )]
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![allow(
// REWRITES-BIONIC-AARCH64-NEXT:     dead_code,
// REWRITES-BIONIC-AARCH64-NEXT:     unused,
// REWRITES-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-AARCH64-NEXT:     non_snake_case,
// REWRITES-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-AARCH64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-AARCH64-NEXT:     unused_comparisons
// REWRITES-BIONIC-AARCH64-NEXT: )]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64: #![allow(
// REWRITES-BIONIC-X86_64-NEXT:     dead_code,
// REWRITES-BIONIC-X86_64-NEXT:     unused,
// REWRITES-BIONIC-X86_64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-X86_64-NEXT:     non_snake_case,
// REWRITES-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-X86_64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-X86_64-NEXT:     unused_comparisons
// REWRITES-BIONIC-X86_64-NEXT: )]
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn main() {
// REWRITES-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-x86_64
