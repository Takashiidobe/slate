// merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang linux-aarch64 aarch64-unknown-linux-gnu clang windows-i686 i686-pc-windows-msvc msvc linux-musl-x86_64 x86_64-unknown-linux-musl clang
#include <stdio.h>

#if defined(_WIN32)
const char *family(void) { return "windows"; }
#else
const char *family(void) { return "unix"; }
#endif

#if defined(__LP64__)
const char *model(void) { return "lp64"; }
#else
const char *model(void) { return "other"; }
#endif

#if !defined(__x86_64__) || !defined(__GLIBC__)
int not_glibc_x86_64;
#endif

int main(void) {
    puts(family());
    puts(model());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: target linux-i686: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: target linux-aarch64: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: target windows-i686: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: target linux-musl-x86_64: 1 compiles
// CHECK-NEXT:   target_properties.c: [] 1 compiled
// CHECK-NEXT: total: 6 compiles across 6 targets
// CHECK-NEXT: unit target_properties: 14 items, 7 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:family: 2 variants
// CHECK-NEXT:   hoisted fn:model: 2 variants
// CHECK-NEXT: projection: 36 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang linux-aarch64 aarch64-unknown-linux-gnu clang windows-i686 i686-pc-windows-msvc msvc linux-musl-x86_64 x86_64-unknown-linux-musl clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "musl")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn puts([[ARG_0:arg[0-9]+]]: *const c_char) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_arch = "aarch64"))]
// CHECK-NEXT: type c_char = i8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "aarch64")]
// CHECK-NEXT: type c_char = u8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: const __SLATE_HOIST_family: &core::ffi::CStr = c"unix";
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: const __SLATE_HOIST_family: &core::ffi::CStr = c"windows";
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn family() -> *const c_char {
// CHECK-NEXT:     __SLATE_HOIST_family.as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_pointer_width = "64", unix))]
// CHECK-NEXT: const __SLATE_HOIST_model: &core::ffi::CStr = c"lp64";
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(windows, target_arch = "x86"))]
// CHECK-NEXT: const __SLATE_HOIST_model: &core::ffi::CStr = c"other";
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn model() -> *const c_char {
// CHECK-NEXT:     __SLATE_HOIST_model.as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(all(target_arch = "x86_64", target_env = "gnu")))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut not_glibc_x86_64: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(family()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(model()) };
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
