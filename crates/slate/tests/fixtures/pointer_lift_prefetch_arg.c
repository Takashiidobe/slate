#include <stdio.h>

static int sum(const int *values, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        __builtin_prefetch(values + i + 4);
        total += values[i];
    }
    return total;
}

static int first(const int *p) {
    const int *aligned = __builtin_assume_aligned(p, 4);
    return *aligned;
}

int main(void) {
    int values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    printf("%d %d\n", sum(values, 8), first(values));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_prefetch([[ARG_0:arg[0-9]+]]: *const (), ...);
// CHECK-NEXT:     fn __builtin_assume_aligned([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: u64, ...) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = values.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn first(p: &i32) -> i32 {
// CHECK-NEXT:     unsafe { *(p as *const i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 8]> = __SlateAlign16(
// CHECK-NEXT:         [1i32, 2i32, 3i32, 4i32, 5i32, 6i32, 7i32, 8i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", sum(values.0.as_slice()), first(&values.0[0usize]));
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
