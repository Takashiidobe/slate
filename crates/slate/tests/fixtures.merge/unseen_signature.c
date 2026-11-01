// merge-sources: units/unseen_signature_helper.c
// merge-expect-exit: 1
// merge-features: alpha beta gamma
#include <stdio.h>

#if defined(ALPHA) && defined(BETA) && !defined(GAMMA)
extern long widen(long);
#else
extern int widen(int);
#endif

int main(void) {
    printf("%d\n", (int)widen(1));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 3 compiles
// CHECK-NEXT:   unseen_signature.c: [alpha,beta,gamma] 8 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT:   units/unseen_signature_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 3 compiles
// CHECK-NEXT:   unseen_signature.c: [alpha,beta,gamma] 8 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT:   units/unseen_signature_helper.c: [] 1 compiled
// CHECK-NEXT: total: 6 compiles across 2 targets
// CHECK-NEXT: unit unseen_signature: 8 items, 4 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 4 variants
// CHECK-NEXT: unit unseen_signature_helper: 1 items, 0 gated, 0 duplicated functions
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 16 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 2 shared symbols, 1 mismatches
// CHECK-NEXT:   linux-x86_64 [alpha,beta]: widen: unseen_signature `extern "C" fn(i64) -> i64` vs unseen_signature_helper `extern "C" fn(i32) -> i32`
// CHECK-NEXT: error: 1 signature mismatches
// CHECK-NEXT: exit: 1
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/unseen_signature_helper.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-expect-exit: 1
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-features: alpha beta gamma
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "unseen_signature_helper.rs"]
// CHECK-NEXT: mod __slate_unit_unseen_signature_helper;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(not(feature = "alpha"), not(feature = "beta"), feature = "gamma"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn widen([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn widen([[ARG_0]]: c_long) -> c_long;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(all(feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         windows,
// CHECK-NEXT:         any(not(feature = "alpha"), not(feature = "beta"), feature = "gamma")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), widen(1i32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(windows, feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), widen((1 as c_long))) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         unix,
// CHECK-NEXT:         any(not(feature = "alpha"), not(feature = "beta"), feature = "gamma")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", unsafe { widen(1i32) });
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(unix, feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", (unsafe { widen((1 as c_long)) }) as i32);
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
// CHECK-NEXT: file unseen_signature_helper.rs
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn widen(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
