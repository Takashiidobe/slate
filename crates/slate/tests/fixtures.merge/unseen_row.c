// merge-features: alpha beta gamma
#include <stdio.h>

void first(void) { puts("same"); }
void second(void) { puts("same"); }

int main(void) {
    int value = 0;
#if defined(ALPHA) && defined(BETA) && !defined(GAMMA)
    value = 7;
    if ((void (*)(void))first == (void (*)(void))second) value = 8;
#endif
    printf("%d\n", value);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   unseen_row.c: [alpha,beta,gamma] 8 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   unseen_row.c: [alpha,beta,gamma] 8 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit unseen_row: 8 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:first: 2 variants
// CHECK-NEXT:   spliced fn:second: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 4 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 16 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[cfg_attr(unix, unsafe(link_section = ".text.slate_distinct.unseen_row.first"))]
// CHECK-NEXT: extern "C-unwind" fn first() {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::puts(c"same".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[cfg_attr(unix, unsafe(link_section = ".text.slate_distinct.unseen_row.second"))]
// CHECK-NEXT: extern "C-unwind" fn second() {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::puts(c"same".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NEXT: //! merge-features: alpha beta gamma
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT:     let mut value: i32 = 7;
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "beta", not(feature = "gamma")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if ({
// CHECK-NEXT:             let __slate_function: Option<unsafe extern "C-unwind" fn()> = Some(first);
// CHECK-NEXT:             __slate_function
// CHECK-NEXT:         }) == Some(second)
// CHECK-NEXT:         {
// CHECK-NEXT:             value = 8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(not(feature = "alpha"), not(feature = "beta"), feature = "gamma"))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), value) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", value);
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
