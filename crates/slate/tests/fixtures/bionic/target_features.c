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
// LOWERING-BIONIC-AARCH64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let _v0: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     __retval = _v0;
// LOWERING-BIONIC-AARCH64-NEXT:     let _v1: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     __retval = _v1;
// LOWERING-BIONIC-AARCH64-NEXT:     let _v2: i32 = __retval;
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit(_v2 as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let _v0: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     __retval = _v0;
// LOWERING-BIONIC-X86_64-NEXT:     let _v1: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     __retval = _v1;
// LOWERING-BIONIC-X86_64-NEXT:     let _v2: i32 = __retval;
// LOWERING-BIONIC-X86_64-NEXT:     std::process::exit(_v2 as i32);
// LOWERING-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT: let mut __retval: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT: __retval = 0;
// REWRITES-BIONIC-AARCH64-NEXT: __retval = 0;
// REWRITES-BIONIC-AARCH64-NEXT: std::process::exit(__retval as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn main() {
// REWRITES-BIONIC-X86_64-NEXT: let mut __retval: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT: __retval = 0;
// REWRITES-BIONIC-X86_64-NEXT: __retval = 0;
// REWRITES-BIONIC-X86_64-NEXT: std::process::exit(__retval as i32);
// REWRITES-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-x86_64
