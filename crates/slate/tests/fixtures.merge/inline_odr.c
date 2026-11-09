// merge-sources: units/inline_odr_helper.c
#include <stdio.h>
#define ODR_DEFINE_HERE
#include "units/inline_odr.h"

int main(void) {
    int first = next_ticket();
    int second = other_unit();
    int third = next_ticket();
    printf("tickets %d %d %d\n", first, second, third);
    printf("twice %d\n", twice_counted(3));
    printf("thrice %d\n", thrice_counted(5));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   inline_odr.c: [] 1 compiled
// CHECK-NEXT:   units/inline_odr_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   inline_odr.c: [] 1 compiled
// CHECK-NEXT:   units/inline_odr_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit inline_odr: 14 items, 4 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:next_ticket: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit inline_odr_helper: 11 items, 7 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:other_unit: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 9 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file inline_odr_helper.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn next_ticket() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: static mut calls__19: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: static mut calls__24: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: static mut count: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "__slate_odr_next_ticket"]
// CHECK-NEXT:     fn next_ticket() -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn other_unit() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"other twice %d\n".as_ptr(), twice_counted(1i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"other thrice %d\n".as_ptr(), thrice_counted(1i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("other twice {}", unsafe { twice_counted(1i32) });
// CHECK-NEXT:         println!("other thrice {}", unsafe { thrice_counted(1i32) });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { next_ticket() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn twice_counted([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn thrice_counted([[ARG_0]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn next_ticket() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { count }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[unsafe(export_name = "__slate_odr_next_ticket")]
// CHECK-NEXT: extern "C-unwind" fn next_ticket() -> i32 {
// CHECK-NEXT:     let [[V_0]]: i32 = (unsafe { count }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "inline_odr_helper.rs"]
// CHECK-NEXT: mod __slate_unit_inline_odr_helper;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn thrice_counted(x: i32) -> i32 {
// CHECK-NEXT:     let [[V_0]]: i32 = (unsafe { calls__24 }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls__24 = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x * 3 + 100 * [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn twice_counted(x: i32) -> i32 {
// CHECK-NEXT:     let [[V_0]]: i32 = (unsafe { calls__19 }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls__19 = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x * 2 + 100 * [[V_0]]
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
// CHECK-NEXT: //! merge-sources: units/inline_odr_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let first: i32 = next_ticket();
// CHECK-NEXT:     let second: i32 = unsafe { other_unit() };
// CHECK-NEXT:     let third: i32 = next_ticket();
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"tickets %d %d %d\n".as_ptr(), first, second, third) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"twice %d\n".as_ptr(), twice_counted(3i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"thrice %d\n".as_ptr(), thrice_counted(5i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("tickets {} {} {}", first, second, third);
// CHECK-NEXT:         println!("twice {}", twice_counted(3i32));
// CHECK-NEXT:         println!("thrice {}", thrice_counted(5i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls__19: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls__24: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut count: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn other_unit() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
