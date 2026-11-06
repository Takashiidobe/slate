#include <stdio.h>

static long total;

#ifdef ALPHA
#define STEP(x) (total += (x) * 2)
#else
#define STEP(x) (total += (x) + 1)
#endif

#ifdef _WIN32
#define MARK(x) do { total ^= (x); } while (0)
#else
#define MARK(x) do { total |= (x); } while (0)
#endif

#define BOTH(x) do { STEP(x); MARK(x); } while (0)
#define ADD_ONE(c) ((c) + 1)

static void families(int n) {
    STEP(n);
    BOTH(n + 1);
    MARK(n);
}

static void coexisting(unsigned char small, int wide) {
    printf("%d %d\n", ADD_ONE(small), ADD_ONE(wide));
}

int main(void) {
    families(3);
    coexisting(200, 41);
    printf("%ld\n", total);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   macro_families.c: [alpha] 2 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   macro_families.c: [alpha] 2 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit macro_families: 16 items, 6 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:coexisting: 2 variants
// CHECK-NEXT: macros: 4 function-like, 4 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro ADD_ONE: 8 lifted, inline:
// CHECK-NEXT:   macro BOTH: 4 lifted, inline:
// CHECK-NEXT:   macro MARK: 8 lifted, inline:
// CHECK-NEXT:   macro STEP: 8 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: macro_rules! STEP {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let __v: c_long = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + ((($x) * 2) as c_long);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "alpha"))]
// CHECK-NEXT: macro_rules! STEP {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let __v: c_long = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + ((($x) + 1) as c_long);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: fn coexisting(small: u8, wide: i32) {
// CHECK-NEXT:     println!("{} {}", ADD_ONE!(small), ADD_ONE__2!(wide));
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! MARK {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: c_long = unsafe { total };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = __v | (($x) as c_long);
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn coexisting(small: u8, wide: i32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %d\n".as_ptr(), ADD_ONE!(small), ADD_ONE__2!(wide)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! MARK {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: c_long = unsafe { total };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = __v ^ (($x) as c_long);
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
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
// CHECK-NEXT:     families(3i32);
// CHECK-NEXT:     coexisting(200u8, 41i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld\n".as_ptr(), total) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn families(n: i32) {
// CHECK-NEXT:     STEP!(n);
// CHECK-NEXT:     BOTH!(n + 1);
// CHECK-NEXT:     MARK!(n);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD_ONE {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         ((($c) as u32) as i32) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD_ONE__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         ($c) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BOTH {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             STEP!($x);
// CHECK-NEXT:             MARK!($x);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut total: c_long = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
