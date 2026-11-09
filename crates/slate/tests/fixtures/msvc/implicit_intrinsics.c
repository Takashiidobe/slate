/* { dg-options "/Zc:preprocessor" } */
#include <stdlib.h>

#if defined(_M_IX86) || defined(_M_X64)
static void check_cpuid(void) {
    unsigned int basic[4];
    unsigned int extended[4];
    __cpuid(basic, 0);
    __cpuidex(extended, 0, 0);
    if (basic[0] == 0 || basic[0] != extended[0] || basic[1] != extended[1])
        abort();
}
#endif

#ifdef _M_X64
static void check_wide(void) {
    unsigned __int64 high;
    unsigned __int64 low = _umul128(0x100000000ull, 0x100000000ull, &high);
    if (low != 0 || high != 1)
        abort();
}
#endif

int main(void) {
#if defined(_M_IX86) || defined(_M_X64)
    check_cpuid();
#endif
#ifdef _M_X64
    check_wide();
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: struct __SlateAlign16<T>(T);
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuid;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuid;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuidex;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuidex;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_umul128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_cpuid() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_cpuid() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut basic: __SlateAlign16<[u32; 4]> = __SlateAlign16([0u32; 4]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut extended: __SlateAlign16<[u32; 4]> = __SlateAlign16([0u32; 4]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe { __cpuid((basic.0.as_mut_ptr() as *mut i32) as *mut i32, 0i32 as i32) };
// I686-PC-WINDOWS-MSVC-NEXT:     let mut basic: [u32; 4] = [0u32; 4];
// I686-PC-WINDOWS-MSVC-NEXT:     let mut extended: [u32; 4] = [0u32; 4];
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe { __cpuid((basic.as_mut_ptr() as *mut i32) as *mut i32, 0i32 as i32) };
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// I686-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (extended.0.as_mut_ptr() as *mut i32) as *mut i32,
// I686-PC-WINDOWS-MSVC-NEXT:             (extended.as_mut_ptr() as *mut i32) as *mut i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if basic.0[0] == 0 || basic.0[0] != extended.0[0] || basic.0[1] != extended.0[1] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_wide() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut high: u64 = 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let low: u64 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _umul128(
// X86_64-PC-WINDOWS-MSVC-NEXT:             4294967296u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             4294967296u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(high) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u64
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if low != 0 || high != 1 {
// I686-PC-WINDOWS-MSVC-NEXT:     if basic[0] == 0 || basic[0] != extended[0] || basic[1] != extended[1] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_cpuid();
// I686-PC-WINDOWS-MSVC-NEXT:     check_cpuid();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_wide();
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
