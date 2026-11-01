// merge-sources: units/two_units_helper.c
// merge-linux-args: -fcommon
#include <stdio.h>

extern int helper(int);
int shared_common;

int main(void) {
    shared_common = 2;
    printf("%d\n", helper(10));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 3 compiles
// CHECK-NEXT:   two_units.c: [] 1 compiled
// CHECK-NEXT:   units/two_units_helper.c: [alpha] 2 compiled
// CHECK-NEXT: target windows-x86_64: 3 compiles
// CHECK-NEXT:   two_units.c: [] 1 compiled
// CHECK-NEXT:   units/two_units_helper.c: [alpha] 2 compiled
// CHECK-NEXT: total: 6 compiles across 2 targets
// CHECK-NEXT: unit two_units: 7 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit two_units_helper: 3 items, 2 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:helper: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 4 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/two_units_helper.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-linux-args: -fcommon
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "two_units_helper.rs"]
// CHECK-NEXT: mod __slate_unit_two_units_helper;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut shared_common: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         shared_common = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), helper(10i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", unsafe { helper(10i32) });
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
// CHECK-NEXT: file two_units_helper.rs
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut shared_common: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut shared_common: i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper(mut x: i32) -> i32 {
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     {
// CHECK-NEXT:         x += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     x * unsafe { shared_common }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
