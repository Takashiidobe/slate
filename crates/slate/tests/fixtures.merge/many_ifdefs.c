#include <stdio.h>

int flags(void) {
    int value = 0;
#ifdef ALPHA
    value += 1;
#endif
#ifdef BETA
    value += 2;
#endif
#ifdef GAMMA
    value += GAMMA * 4;
#endif
    return value;
}

int main(void) {
    printf("%d\n", flags());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 6 compiles
// CHECK-NEXT:   many_ifdefs.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: target windows-x86_64: 6 compiles
// CHECK-NEXT:   many_ifdefs.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: total: 12 compiles across 2 targets
// CHECK-NEXT: unit many_ifdefs: 5 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:flags: 6 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn flags() -> i32 {
// CHECK-NEXT:     #[cfg(any(feature = "alpha", feature = "beta", feature = "gamma"))]
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 3 * 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(not(feature = "alpha"), not(feature = "beta"), not(feature = "gamma")))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), flags()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", flags());
// CHECK-NEXT:     }
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
