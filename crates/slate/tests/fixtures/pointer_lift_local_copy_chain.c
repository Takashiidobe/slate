// SLATE-ENV: SLATE_IR_POINTERS=1
#include <stdlib.h>

static int sum(const int *p, int n) {
    const int *t1 = p;
    const int *t2 = t1;
    int s = 0;
    for (int i = 0; i < n; i++)
        s += t2[i];
    return s;
}

int main(void) {
    int a[4] = {1, 2, 3, 4};
    if (sum(a, 4) != 10)
        abort();
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! SLATE-ENV: SLATE_IR_POINTERS=1
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(p: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = p.len() as i32;
// CHECK-NEXT:     let t1: &[i32] = p;
// CHECK-NEXT:     let t2: &[i32] = t1;
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         s += t2[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     if sum(a.0.as_slice()) != 10 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
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
