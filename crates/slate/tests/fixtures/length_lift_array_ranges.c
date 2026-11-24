#include <stdio.h>

static int sum(const int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; ++i)
    total += values[i];
  return total;
}

static int previous(const int *values, int count) {
  return values[count - 1];
}

int main(void) {
  int values[4] = {10, 20, 30, 40};
  printf("%d %d %d %d %d %d %d\n", sum(values, 4), sum(values + 1, 3),
         sum((values + 3) - 2, 2), sum(&values[2], 1), sum(values, 2),
         sum(values + 4, 0), previous(values + 1, 0));
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
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let count: i32 = values.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn previous(values: *const i32, count: i32) -> i32 {
// CHECK-NEXT:     unsafe { *values.offset((count - 1) as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16(
// CHECK-NEXT:         [10i32, 20i32, 30i32, 40i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg6 = previous(
// CHECK-NEXT:         unsafe { values.0.as_mut_ptr().add(1) as *const i32 },
// CHECK-NEXT:         0i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {}",
// CHECK-NEXT:         sum(values.0.as_slice()),
// CHECK-NEXT:         sum(&values.0[1usize..4usize]),
// CHECK-NEXT:         sum(&values.0[1usize..3usize]),
// CHECK-NEXT:         sum(&values.0[2usize..3usize]),
// CHECK-NEXT:         sum(&values.0[0usize..2usize]),
// CHECK-NEXT:         sum(&values.0[4usize..4usize]),
// CHECK-NEXT:         __slate_printf_arg6
// CHECK-NEXT:     );
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
