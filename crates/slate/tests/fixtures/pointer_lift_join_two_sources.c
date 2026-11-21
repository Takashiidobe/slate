#include <stdio.h>

static int read_then_store(const int *a, int *b, int which) {
    const int *q = a;
    int s = *q;
    if (which)
        q = b;
    s += *q;
    *b = s;
    return s;
}

int main(void) {
    int a = 2, b = 5;
    int r = read_then_store(&a, &b, 1);
    printf("%d %d %d\n", r, a, b);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn read_then_store(a: &i32, b: *mut i32, which: i32) -> i32 {
// CHECK-NEXT:     let mut q: *const i32 = a as *const i32;
// CHECK-NEXT:     let mut s: i32 = unsafe { *q };
// CHECK-NEXT:     if which != 0 {
// CHECK-NEXT:         q = b as *const i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     s += unsafe { *q };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *b = s;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 2;
// CHECK-NEXT:     let mut b: i32 = 5;
// CHECK-NEXT:     let r: i32 = read_then_store(
// CHECK-NEXT:         unsafe { &(*(core::ptr::addr_of_mut!(a) as *const i32)) },
// CHECK-NEXT:         core::ptr::addr_of_mut!(b),
// CHECK-NEXT:         1i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", r, a, b);
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
