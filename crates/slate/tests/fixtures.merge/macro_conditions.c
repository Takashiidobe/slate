#include <stdio.h>

static int verbose = 2;

#define Tracecv(c, x) do { if (verbose > 1 && (c)) printf x; } while (0)
#define WHEN(c) (verbose > 1 && (c))
#define EXPLICIT(c) (verbose > 1 && (c) != 0)
#define TWICE(c) ((c) ? (c) + 1 : 0)
#define UNLESS(c) (verbose > 1 && !(c))

int main(void) {
    int i = 1;
    long m = 0;
    double d = 0.5;
    const char *p = "x";
    Tracecv(i, ("int\n"));
    Tracecv(i > 0, ("bool %d\n", i));
    Tracecv(m, ("long\n"));
    Tracecv(p, ("pointer %s\n", p));
    Tracecv(d, ("double\n"));
    printf("%d %d %d %d\n", WHEN(i), WHEN(i > 0), WHEN(m), WHEN(p));
    printf("%d %d\n", EXPLICIT(i), EXPLICIT(m));
    printf("%d %ld\n", TWICE(i), TWICE(m));
    printf("%d %d\n", UNLESS(m), UNLESS(p));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_conditions.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_conditions.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_conditions: 14 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 5 function-like, 5 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro EXPLICIT: 4 lifted, inline:
// CHECK-NEXT:   macro TWICE: 4 lifted, inline:
// CHECK-NEXT:   macro Tracecv: 10 lifted, inline:
// CHECK-NEXT:   macro UNLESS: 4 lifted, inline:
// CHECK-NEXT:   macro WHEN: 8 lifted, inline:
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
// CHECK-NEXT:     let i: i32 = 1;
// CHECK-NEXT:     let m: c_long = 0;
// CHECK-NEXT:     let d: f64 = 0.5f64;
// CHECK-NEXT:     let p: *const c_char = c"x".as_ptr();
// CHECK-NEXT:     Tracecv!(i != 0, c"int\n".as_ptr() as *mut c_char);
// CHECK-NEXT:     Tracecv!(i > 0, c"bool %d\n".as_ptr() as *mut c_char, i);
// CHECK-NEXT:     Tracecv!(m != 0, c"long\n".as_ptr() as *mut c_char);
// CHECK-NEXT:     Tracecv!(!p.is_null(), c"pointer %s\n".as_ptr() as *mut c_char, p);
// CHECK-NEXT:     Tracecv!(d != 0.0f64, c"double\n".as_ptr() as *mut c_char);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 WHEN!(i != 0) as i32,
// CHECK-NEXT:                 WHEN!(i > 0) as i32,
// CHECK-NEXT:                 WHEN!(m != 0) as i32,
// CHECK-NEXT:                 WHEN!(!p.is_null()) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 EXPLICIT__2!(i) as i32,
// CHECK-NEXT:                 EXPLICIT!(m) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             WHEN!(i != 0) as i32,
// CHECK-NEXT:             WHEN!(i > 0) as i32,
// CHECK-NEXT:             WHEN!(m != 0) as i32,
// CHECK-NEXT:             WHEN!(!p.is_null()) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{} {}", EXPLICIT__2!(i) as i32, EXPLICIT!(m) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %ld\n".as_ptr(), TWICE__2!(i), TWICE!(m)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"%d %d\n".as_ptr(), UNLESS!(m) as i32, UNLESS__2!(p) as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", UNLESS!(m) as i32, UNLESS__2!(p) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! EXPLICIT {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         (unsafe { verbose }) > 1 && ($c) != 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! EXPLICIT__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         (unsafe { verbose }) > 1 && ($c) != 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         if ($c) != 0 { ($c) + 1 } else { 0 }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TWICE__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         if ($c) != 0 { ($c) + 1 } else { 0 }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! Tracecv {
// CHECK-NEXT:     ($c:expr, $x__0:expr $(, $x:expr)*) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if (unsafe { verbose }) > 1 && $c {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { slate_support::printf(($x__0) as *const c_char, $($x),*) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! UNLESS {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         (unsafe { verbose }) > 1 && ($c) == 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! UNLESS__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         (unsafe { verbose }) > 1 && $c.is_null()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WHEN {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         (unsafe { verbose }) > 1 && $c
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 2;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
