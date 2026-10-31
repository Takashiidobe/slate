#include <stdio.h>

#ifdef _WIN32
#define NAME "windows"
#else
#define NAME "unix"
#endif

const char *platform(void) { return NAME; }

#ifdef ALPHA
const char *platform_alpha(void) { return NAME "-alpha"; }
#endif

int main(void) {
    puts(platform());
#ifdef ALPHA
    puts(platform_alpha());
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   by_target.c: [alpha] 2 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   by_target.c: [alpha] 2 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit by_target: 9 items, 6 gated, 3 duplicated functions
// CHECK-NEXT:   duplicated fn:platform: 2 variants
// CHECK-NEXT:   duplicated fn:platform_alpha: 2 variants
// CHECK-NEXT:   duplicated fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn puts([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn platform() -> *const i8 {
// CHECK-NEXT:     c"unix".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn platform() -> *const i8 {
// CHECK-NEXT:     c"windows".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn platform_alpha() -> *const i8 {
// CHECK-NEXT:     c"unix-alpha".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     windows,
// CHECK-NEXT:     target_env = "msvc",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn platform_alpha() -> *const i8 {
// CHECK-NEXT:     c"windows-alpha".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "alpha"))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(platform()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(platform()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(platform_alpha()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
