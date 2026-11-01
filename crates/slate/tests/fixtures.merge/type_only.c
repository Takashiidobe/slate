#include <stdio.h>

struct state {
    int head;
#ifdef ALPHA
    long extra;
#endif
};

int state_size(void) { return (int)sizeof(struct state); }

int unrelated(int x) { return x + 1; }

int main(void) {
    printf("%d %d\n", state_size() > 4, unrelated(1));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   type_only.c: [alpha] 2 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   type_only.c: [alpha] 2 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit type_only: 10 items, 5 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:state_size: 3 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
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
// CHECK-NEXT: #[cfg(not(feature = "alpha"))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(unix, feature = "alpha"))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 16;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(windows, feature = "alpha"))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 8;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn state_size() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_state_size
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn unrelated(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 (state_size() > 4) as i32,
// CHECK-NEXT:                 unrelated(1i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", (state_size() > 4) as i32, unrelated(1i32));
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
