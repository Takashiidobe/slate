#include <stdio.h>

#ifdef ALPHA
static int only_alpha(void) { return 7; }
#endif

#ifdef BETA
int table[2] = {1, 2};
#else
int table[3] = {1, 2, 3};
#endif

int main(void) {
#ifdef ALPHA
    printf("%d\n", only_alpha());
#endif
    printf("%d\n", (int)(sizeof(table) / sizeof(table[0])));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 4 compiles
// CHECK-NEXT:   presence.c: [alpha,beta] 4 compiled
// CHECK-NEXT: target windows-x86_64: 4 compiles
// CHECK-NEXT:   presence.c: [alpha,beta] 4 compiled
// CHECK-NEXT: total: 8 compiles across 2 targets
// CHECK-NEXT: unit presence: 8 items, 5 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 8 variants
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
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: fn only_alpha() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 3] = [1, 2, 3];
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 2] = [1, 2];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(all(windows, feature = "alpha"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), only_alpha()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(windows, feature = "alpha"))]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(all(windows, not(feature = "beta")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), 3i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(windows, feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), 2i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(unix, feature = "alpha"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", only_alpha());
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(unix, not(feature = "beta")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", 3i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(unix, feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", 2i32);
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
