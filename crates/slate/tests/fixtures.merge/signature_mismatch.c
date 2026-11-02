// merge-sources: units/signature_mismatch_helper.c
// merge-expect-exit: 1
#include <stdio.h>

struct pair {
    int first;
};

extern int widen(int);
extern struct pair make_pair(int);
extern int table_size;

int main(void) {
    printf("%d %d %d\n", widen(1), make_pair(2).first, table_size);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 7 compiles
// CHECK-NEXT:   signature_mismatch.c: [] 1 compiled
// CHECK-NEXT:   units/signature_mismatch_helper.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: target windows-x86_64: 7 compiles
// CHECK-NEXT:   signature_mismatch.c: [] 1 compiled
// CHECK-NEXT:   units/signature_mismatch_helper.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: total: 14 compiles across 2 targets
// CHECK-NEXT: unit signature_mismatch: 9 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit signature_mismatch_helper: 8 items, 5 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:widen: 2 variants
// CHECK-NEXT:   spliced record:pair: 2 variants
// CHECK-NEXT:   spliced fn:make_pair: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 6 shared symbols, 4 mismatches
// CHECK-NEXT:   linux-x86_64 [beta]: make_pair: signature_mismatch `extern "C" fn(i32) -> struct{i32}` vs signature_mismatch_helper `extern "C" fn(i32) -> struct{i32, i64}`
// CHECK-NEXT:   linux-x86_64 [gamma]: table_size: signature_mismatch `i32` vs signature_mismatch_helper `i64`
// CHECK-NEXT:   linux-x86_64 [alpha]: widen: signature_mismatch `extern "C" fn(i32) -> i32` vs signature_mismatch_helper `extern "C" fn(i64) -> i64`
// CHECK-NEXT:   windows-x86_64 [beta]: make_pair: signature_mismatch `extern "C" fn(i32) -> struct{i32}` vs signature_mismatch_helper `extern "C" fn(i32) -> struct{i32, i32}`
// CHECK-NEXT: error: 4 signature mismatches
// CHECK-NEXT: exit: 1
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "signature_mismatch_helper.rs"]
// CHECK-NEXT: mod __slate_unit_signature_mismatch_helper;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
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
// CHECK-NEXT: //! merge-sources: units/signature_mismatch_helper.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-expect-exit: 1
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 widen(1i32),
// CHECK-NEXT:                 (*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = make_pair(2i32);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .first,
// CHECK-NEXT:                 table_size,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             unsafe { widen(1i32) },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = make_pair(2i32);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .first
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe { table_size }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut table_size: i32;
// CHECK-NEXT:     fn widen([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn make_pair([[ARG_0]]: i32) -> pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: file signature_mismatch_helper.rs
// CHECK-NEXT: #[cfg(any(feature = "alpha", feature = "beta", feature = "gamma"))]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: type __SlateHoist_widen = c_long;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "gamma")]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_size: c_long = 3;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "alpha"))]
// CHECK-NEXT: type __SlateHoist_widen = i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "gamma"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_size: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     second: c_long,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn make_pair(x: i32) -> pair {
// CHECK-NEXT:     #[cfg(not(feature = "beta"))]
// CHECK-NEXT:     let mut pair: pair = pair { first: 0 };
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     let mut pair: pair = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].first = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     pair.first = x;
// CHECK-NEXT:     return pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn widen(x: __SlateHoist_widen) -> __SlateHoist_widen {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
