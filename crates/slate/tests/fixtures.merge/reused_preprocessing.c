#include <stdio.h>

#ifdef ALPHA
#define UNUSED_WIDTH 8
#endif

#if defined(ALPHA) && defined(GAMMA)
#error "alpha and gamma preprocess like gamma but must stay infeasible"
#endif

int value(void) {
#ifdef BETA
    return 2;
#else
    return 1;
#endif
}

int main(void) {
    printf("%d\n", value());
#ifdef GAMMA
    printf("gamma\n");
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 4 compiles
// CHECK-NEXT:   reused_preprocessing.c: [alpha,beta,gamma] 5 compiled
// CHECK-NEXT:     2 assignments preprocess like a compiled one
// CHECK-NEXT:     infeasible [alpha,gamma]: {{.*}}tests/fixtures.merge/reused_preprocessing.c: analyze {{.*}}tests/fixtures.merge/reused_preprocessing.c: #error "alpha and gamma preprocess like gamma but must stay infeasible"
// CHECK-NEXT: target windows-x86_64: 4 compiles
// CHECK-NEXT:   reused_preprocessing.c: [alpha,beta,gamma] 5 compiled
// CHECK-NEXT:     2 assignments preprocess like a compiled one
// CHECK-NEXT:     infeasible [alpha,gamma]: {{.*}}tests/fixtures.merge/reused_preprocessing.c: analyze {{.*}}tests/fixtures.merge/reused_preprocessing.c: #error "alpha and gamma preprocess like gamma but must stay infeasible"
// CHECK-NEXT: total: 8 compiles across 2 targets
// CHECK-NEXT: unit reused_preprocessing: 7 items, 3 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:value: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 4 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 10 checked, 2 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: const __SLATE_HOIST_value: i32 = 2;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: const __SLATE_HOIST_value: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn value() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_value
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
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), value()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(windows, feature = "gamma"))]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(all(windows, feature = "gamma"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"gamma\n".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", value());
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(unix, feature = "gamma"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("gamma");
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
