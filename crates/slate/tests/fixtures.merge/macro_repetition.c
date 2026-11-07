#include <stdio.h>

static int verbose = 2;

#define Tracev(x) do { if (verbose > 1) printf x; } while (0)
#define SUM_OF(args) sum3 args
#define REPORT(tag, x) do { printf("%s: ", tag); printf x; } while (0)

static int sum3(int a, int b, int c) { return a + b + c; }

int main(void) {
    int i = 1, j = 2;
    long m = 3;
    const char *s = "str";
    unsigned char narrow = 7;
    float f = 1.5f;
    Tracev(("plain\n"));
    Tracev(("%d\n", i));
    Tracev(("%d %d\n", i, j));
    Tracev(("%d %d %ld %s\n", i, j, m, s));
    Tracev(("%d %d %d %d %d %d %d\n", i, j, i, j, i, j, i));
    Tracev(("%d\n", narrow));
    Tracev(("%d %.1f %d\n", narrow, f, i));
    REPORT("one", ("%d\n", i));
    REPORT("two", ("%d %ld\n", j, m));
    printf("%d\n", SUM_OF((i, j, 3)));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_repetition.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_repetition.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_repetition: 10 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 3 function-like, 3 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro REPORT: 4 lifted, inline:
// CHECK-NEXT:   macro SUM_OF: 2 lifted, inline:
// CHECK-NEXT:   macro Tracev: 14 lifted, inline:
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
// CHECK-NEXT:     let j: i32 = 2;
// CHECK-NEXT:     let m: c_long = 3;
// CHECK-NEXT:     let s: *const c_char = c"str".as_ptr();
// CHECK-NEXT:     let narrow: u8 = 7;
// CHECK-NEXT:     let f: f32 = 1.5f32;
// CHECK-NEXT:     Tracev!(c"plain\n".as_ptr() as *mut c_char);
// CHECK-NEXT:     Tracev!(c"%d\n".as_ptr() as *mut c_char, i);
// CHECK-NEXT:     Tracev!(c"%d %d\n".as_ptr() as *mut c_char, i, j);
// CHECK-NEXT:     Tracev!(c"%d %d %ld %s\n".as_ptr() as *mut c_char, i, j, m, s);
// CHECK-NEXT:     Tracev!(
// CHECK-NEXT:         c"%d %d %d %d %d %d %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         i,
// CHECK-NEXT:         j,
// CHECK-NEXT:         i,
// CHECK-NEXT:         j,
// CHECK-NEXT:         i,
// CHECK-NEXT:         j,
// CHECK-NEXT:         i
// CHECK-NEXT:     );
// CHECK-NEXT:     Tracev!(c"%d\n".as_ptr() as *mut c_char, narrow as i32);
// CHECK-NEXT:     Tracev!(
// CHECK-NEXT:         c"%d %.1f %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         narrow as i32,
// CHECK-NEXT:         f as f64,
// CHECK-NEXT:         i
// CHECK-NEXT:     );
// CHECK-NEXT:     REPORT!(
// CHECK-NEXT:         c"one".as_ptr() as *mut c_char,
// CHECK-NEXT:         c"%d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         i
// CHECK-NEXT:     );
// CHECK-NEXT:     REPORT!(
// CHECK-NEXT:         c"two".as_ptr() as *mut c_char,
// CHECK-NEXT:         c"%d %ld\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         j,
// CHECK-NEXT:         m
// CHECK-NEXT:     );
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), SUM_OF!(i, j, 3i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", SUM_OF!(i, j, 3i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum3(a: i32, b: i32, c: i32) -> i32 {
// CHECK-NEXT:     a + b + c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! REPORT {
// CHECK-NEXT:     ($tag:expr, $x__0:expr $(, $x:expr)*) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%s: ".as_ptr(), $tag) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(($x__0) as *const c_char, $($x),*) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SUM_OF {
// CHECK-NEXT:     ($($args:expr),*) => {
// CHECK-NEXT:         sum3($($args),*)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! Tracev {
// CHECK-NEXT:     ($x__0:expr $(, $x:expr)*) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if (unsafe { verbose }) > 1 {
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
