#include <stdio.h>

static int sum_then_next(int *p) {
    int s = 0;
    int i;
    for (i = 0; i < 4; i++) {
        s += p[i];
    }
    return s + p[i];
}

static int two_loops(int *p, int *q) {
    int s = 0;
    for (int i = 0; i < 4; i++) {
        s += p[i];
    }
    for (int i = 0; i < 8; i++) {
        q[i] = i;
    }
    return s;
}

static int shifted(int *p) {
    int s = 0;
    int i = 0;
    do {
        i++;
        s += p[i];
    } while (i < 4);
    return s;
}

int main(void) {
    int a[5] = {1, 2, 3, 4, 5};
    int small[4] = {1, 2, 3, 4};
    int big[8];
    printf("%d\n", sum_then_next(a));
    printf("%d\n", two_loops(small, big));
    printf("%d\n", big[7]);
    printf("%d\n", shifted(a));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_then_next(p: &[i32]) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < 4 {
// CHECK-NEXT:         s += p[i as usize];
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s + p[i as usize]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn two_loops(p: &[i32], q: &mut [i32]) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         s += p[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         q[i as usize] = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn shifted(p: &[i32]) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:         s += p[i as usize];
// CHECK-NEXT:         if i >= 4 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 5]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32, 5i32]);
// CHECK-NEXT:     let mut small: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     let mut big: __SlateAlign16<[i32; 8]> = __SlateAlign16([0i32; 8]);
// CHECK-NEXT:     println!("{}", sum_then_next(a.0.as_slice()));
// CHECK-NEXT:     println!("{}", two_loops(small.0.as_slice(), big.0.as_mut_slice()));
// CHECK-NEXT:     println!("{}", big.0[7]);
// CHECK-NEXT:     println!("{}", shifted(a.0.as_slice()));
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
