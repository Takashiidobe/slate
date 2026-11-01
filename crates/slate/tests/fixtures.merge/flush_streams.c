// merge-extra-targets: android-x86_64 x86_64-linux-android clang
#include <stdlib.h>
#include <stdio.h>

static int verbose = 1;

#define TRACE(x) { if (verbose > 0) fprintf x; }
#define CHECK(c, m) { if (!(c)) fputs(m, stderr); }

static void report(int value) {
    fprintf(stderr, "value %d\n", value);
}

int main(void) {
    printf("one %d\n", 1);
    printf("two %s\n", "x");
    TRACE((stderr, "traced %d\n", 2));
    CHECK(verbose == 0, "checked\n");
    fputs("three\n", stdout);
    fprintf(stdout, "four %s\n", "x");
    printf("five %d\n", 5);
    fwrite("written\n", 1, 8, stderr);
    fputc('6', stdout);
    fputc('\n', stderr);
    printf("seven %d\n", 7);
    printf("eight");
    report(8);
    printf(" nine\n");
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit flush_streams: 12 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:report: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: android-x86_64 x86_64-linux-android clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TRACE {
// CHECK-NEXT:     ($($x:expr),*) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::fprintf($($x),*) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($c:expr, $m:expr) => {
// CHECK-NEXT:         if !$c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::fputs($m, slate_support::stderr()) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fwrite(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *const (),
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: size_t,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: size_t,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE,
// CHECK-NEXT:     ) -> size_t;
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: fn report(value: i32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::fprintf(slate_support::stderr(), c"value %d\n".as_ptr(), value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::fprintf(slate_support::stderr(), c"value %d\n".as_ptr(), value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"one %d\n".as_ptr(), 1i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"two %s\n".as_ptr(), c"x".as_ptr() as *mut c_char) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("one {}", 1i32);
// CHECK-NEXT:         unsafe { slate_support::printf(c"two %s\n".as_ptr(), c"x".as_ptr() as *mut c_char) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     TRACE!(slate_support::stderr(), c"traced %d\n".as_ptr(), 2i32);
// CHECK-NEXT:     CHECK!((unsafe { verbose }) == 0, c"checked\n".as_ptr());
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"three\n".as_ptr(), slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"four %s\n".as_ptr(),
// CHECK-NEXT:                 c"x".as_ptr() as *mut c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"five %d\n".as_ptr(), 5i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"four %s\n".as_ptr(),
// CHECK-NEXT:                 c"x".as_ptr() as *mut c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fwrite(
// CHECK-NEXT:                 b"written\n\0".as_ptr() as *const (),
// CHECK-NEXT:                 1u64,
// CHECK-NEXT:                 8u64,
// CHECK-NEXT:                 slate_support::stderr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("five {}", 5i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fwrite(
// CHECK-NEXT:                 b"written\n\0".as_ptr() as *const (),
// CHECK-NEXT:                 (1 as c_ulong),
// CHECK-NEXT:                 (8 as c_ulong),
// CHECK-NEXT:                 slate_support::stderr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputc(54i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputc(10i32, slate_support::stderr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"seven %d\n".as_ptr(), 7i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("seven {}", 7i32);
// CHECK-NEXT:         print!("eight");
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"eight".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     report(8i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c" nine\n".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(" nine");
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
