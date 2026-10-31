#include <stdio.h>

#ifdef _WIN32
#define FIRST 1
#define SECOND 2
#else
#define FIRST 3
#define SECOND 4
#endif

int two_values(void) { return FIRST * 10 + SECOND; }

int two_statements(int value) {
#ifdef _WIN32
    value += 1;
#endif
    return value;
}

unsigned long long mixed_types(unsigned long long wide, int narrow) {
    return wide == FIRST ? (unsigned long long)(narrow + FIRST) : 0;
}

int main(void) {
    return two_values() + two_statements(1) + (int)mixed_types(3, 1) - 34 - 1 - 4;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   hoist_fallback.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   hoist_fallback.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit hoist_fallback: 7 items, 4 gated, 2 duplicated functions
// CHECK-NEXT:   duplicated fn:two_values: 2 variants
// CHECK-NEXT:   duplicated fn:mixed_types: 2 variants
// CHECK-NEXT:   spliced fn:two_statements: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn two_values() -> i32 {
// CHECK-NEXT:     3 * 10 + 4
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn two_values() -> i32 {
// CHECK-NEXT:     1 * 10 + 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn two_statements(mut value: i32) -> i32 {
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mixed_types(wide: u64, narrow: i32) -> u64 {
// CHECK-NEXT:     if wide == 3 {
// CHECK-NEXT:         ((narrow + 3) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mixed_types(wide: u64, narrow: i32) -> u64 {
// CHECK-NEXT:     if wide == 1 {
// CHECK-NEXT:         ((narrow + 1) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     two_values() + two_statements(1i32) + ((mixed_types(3u64, 1i32) as u32) as i32) - 34 - 1 - 4
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
