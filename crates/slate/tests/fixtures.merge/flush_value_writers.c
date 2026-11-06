// merge-extra-targets: freebsd-x86_64 x86_64-unknown-freebsd clang
#include <stdio.h>

int main(void) {
    printf("one %d\n", 1);
    putc('2', stdout);
    putc_unlocked('\n', stdout);
    int written = printf("three\n");
    printf("written %d\n", written);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   flush_value_writers.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   flush_value_writers.c: [] 1 compiled
// CHECK-NEXT: target freebsd-x86_64: 1 compiles
// CHECK-NEXT:   flush_value_writers.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit flush_value_writers: 8 items, 3 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 3 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn putc_unlocked([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __isthreaded: i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn putc_unlocked(...) -> i32;
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
// CHECK-NEXT: //! merge-extra-targets: freebsd-x86_64 x86_64-unknown-freebsd clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "freebsd")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"one %d\n".as_ptr(), 1i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("one {}", 1i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "freebsd")]
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if (unsafe { __isthreaded }) == 0 {
// CHECK-NEXT:         unsafe { slate_support::__sputc(50i32, slate_support::stdout()) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { slate_support::putc(50i32, slate_support::stdout()) }
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(target_env = "gnu"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(target_env = "gnu"))]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(not(target_os = "freebsd"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::putc(50i32, slate_support::stdout()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "freebsd")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::__sputc(10i32, slate_support::stdout()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(not(target_os = "freebsd"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { putc_unlocked(10i32, slate_support::stdout()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     let written: i32 = unsafe { slate_support::printf(c"three\n".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"written %d\n".as_ptr(), written) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("written {}", written);
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
