#include <stdio.h>

int late(int x);
int early(int x);
int counted(int x);

/* section A */
int early(int x) { return x + 1; }

/* counted doc */
int counted(int x) {
    static int calls = 0;
    calls++;
    return x + calls;
}

/* section B */
int late(int x) { return x * 2; }

int main(void) {
    printf("%d %d %d\n", early(1), counted(1), late(2));
    return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// section A
// CHECK-NEXT: fn early(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// counted doc
// CHECK-NEXT: fn counted(x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     x + unsafe { calls }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: /// section B
// CHECK-NEXT: fn late(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         early(1 as i32),
// CHECK-NEXT:         counted(1 as i32),
// CHECK-NEXT:         late(2 as i32)
// CHECK-NEXT:     );
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
