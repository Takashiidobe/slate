#include <stdio.h>

/* ---- first section ---- */

int a1 = 1;

static int s1(void) { return a1; }

int e1(void) { return s1(); }

/* ---- second section ---- */

int a2 = 2;

/* second static */
static int s2(void) { return a2; }

int e2(void) { return s2(); }

int main(void) {
    printf("%d\n", e1() + e2());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! ---- first section ----
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a1: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: fn s1() -> i32 {
// CHECK-NEXT:     unsafe { a1 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn e1() -> i32 {
// CHECK-NEXT:     s1()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // ---- second section ----
// CHECK-NEXT: static mut a2: i32 = 2;
// CHECK-EMPTY:
// CHECK-NEXT: /// second static
// CHECK-NEXT: fn s2() -> i32 {
// CHECK-NEXT:     unsafe { a2 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn e2() -> i32 {
// CHECK-NEXT:     s2()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", e1() + e2());
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
