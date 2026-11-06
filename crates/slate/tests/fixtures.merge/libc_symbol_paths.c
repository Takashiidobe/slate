// merge-extra-targets: musl-x86_64 x86_64-unknown-linux-musl clang android-x86_64 x86_64-linux-android clang darwin-x86_64 x86_64-apple-darwin clang freebsd-x86_64 x86_64-unknown-freebsd clang
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static long number(const char *text) {
  return strtol(text, NULL, 10);
}

static int retry_code(void) {
  return EAGAIN;
}

int main(void) {
  printf("%ld %d\n", number("41") + 1, retry_code() > 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: target musl-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: target darwin-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: target freebsd-x86_64: 1 compiles
// CHECK-NEXT:   libc_symbol_paths.c: [] 1 compiled
// CHECK-NEXT: total: 6 compiles across 6 targets
// CHECK-NEXT: unit libc_symbol_paths: 11 items, 4 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:number: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_os = "macos"))]
// CHECK-NEXT: fn number(text: *const c_char) -> c_long {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strtol(
// CHECK-NEXT:             text as *const core::ffi::c_char,
// CHECK-NEXT:             std::ptr::null_mut::<*mut c_char>() as *mut *mut core::ffi::c_char,
// CHECK-NEXT:             10i32 as i32,
// CHECK-NEXT:         ) as c_long
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: fn number(text: *const c_char) -> c_long {
// CHECK-NEXT:     unsafe { strtol(text, std::ptr::null_mut::<*mut c_char>(), 10i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strtol([[ARG_0:arg[0-9]+]]: *const c_char, [[ARG_1:arg[0-9]+]]: *mut *mut c_char, [[ARG_2:arg[0-9]+]]: i32) -> c_long;
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
// CHECK-NEXT: //! merge-extra-targets: musl-x86_64 x86_64-unknown-linux-musl clang android-x86_64 x86_64-linux-android clang darwin-x86_64 x86_64-apple-darwin clang freebsd-x86_64 x86_64-unknown-freebsd clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "musl"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "macos"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "freebsd")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %d\n".as_ptr(),
// CHECK-NEXT:             number(c"41".as_ptr()) + 1,
// CHECK-NEXT:             (retry_code() > 0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn retry_code() -> i32 {
// CHECK-NEXT:     slate_support::EAGAIN as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: #[cfg(not(target_os = "macos"))]
// CHECK-NEXT: use slate_support::strtol;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
