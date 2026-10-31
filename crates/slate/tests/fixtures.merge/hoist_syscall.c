#include <stdio.h>
#include <sys/syscall.h>

int main(void) {
    printf("%d\n", SYS_preadv);
    return 0;
}

// merge-second-target: linux-aarch64 aarch64-unknown-linux-gnu clang

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   hoist_syscall.c: [] 1 compiled
// CHECK-NEXT: target linux-aarch64: 1 compiles
// CHECK-NEXT:   hoist_syscall.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit hoist_syscall: 7 items, 4 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: const __SLATE_HOIST___slate_main: i32 = 295i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: const __SLATE_HOIST___slate_main: i32 = 69i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", __SLATE_HOIST___slate_main);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // merge-second-target: linux-aarch64 aarch64-unknown-linux-gnu clang
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
