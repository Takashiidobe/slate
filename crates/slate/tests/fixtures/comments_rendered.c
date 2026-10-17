/*
 * Prologue block.
 * ==========
 */
#include <stdio.h>

/** Doc for point. */
struct point {
    int x; /* the x */
    /* the y */
    int y;
};

/* limit of things */
static const int limit = 3; /* trailing limit */

enum color {
    RED,   /* red one */
    GREEN, /* green one */
};

/**
 * Adds.
 *
 * @param a left
 */
static int add(int a /* left */, int b /* right */) {
    int r = a + b; /* sum */
    /* FALLTHROUGH */
    return r;
}

int main(void) {
    struct point p = {1, 2};
    printf("%d %d\n", add(p.x, p.y), limit + GREEN);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Prologue block.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Doc for point.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct point {
// CHECK-NEXT:     /// the x
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     /// the y
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// limit of things
// CHECK-NEXT: /// trailing limit
// CHECK-NEXT: static mut limit: i32 = 3 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: /// Adds.
// CHECK-NEXT: ///
// CHECK-NEXT: /// @param a left
// CHECK-NEXT: fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     // left
// CHECK-NEXT:     // right
// CHECK-NEXT:     let r: i32 = a + b; // sum
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: point = point { x: 1, y: 2 };
// CHECK-NEXT:     println!("{} {}", add(p.x, p.y), (unsafe { limit }) + 1);
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
