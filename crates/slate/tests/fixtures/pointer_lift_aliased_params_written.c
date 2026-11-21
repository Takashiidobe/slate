#include <stdio.h>

static void add_into(int *dst, const int *src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] += src[i];
}

static int total(const int *a, const int *b) { return *a + *b; }

int main(void) {
    int v[3] = {1, 2, 3};
    add_into(v, v, 3);
    int x = 4;
    printf("%d %d %d %d\n", v[0], v[1], v[2], total(&x, &x));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn add_into(dst: *mut i32, src: *const i32, n: i32) {
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { dst.offset(i as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = *[[V_0]] + *src.offset(i as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn total(a: &i32, b: &i32) -> i32 {
// CHECK-NEXT:     *a + *b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     add_into(v.as_mut_ptr() as *mut i32, v.as_mut_ptr() as *const i32, 3i32);
// CHECK-NEXT:     let x: i32 = 4;
// CHECK-NEXT:     println!("{} {} {} {}", v[0], v[1], v[2], total(&x, &x));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
