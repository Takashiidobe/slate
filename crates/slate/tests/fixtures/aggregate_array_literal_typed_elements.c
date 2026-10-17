#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct pair {
  const char *name;
  const char *value;
};

static int count_null_pairs(void) {
  struct pair pairs[] = {{NULL, NULL}};
  int         total   = 0;
  for (int i = 0; i < 1; i++) {
    if (pairs[i].name == NULL && pairs[i].value == NULL) {
      total++;
    }
  }
  return total;
}

static int count_true_flags(void) {
  bool values[] = {true, false};
  int  total    = 0;
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    if (values[i]) {
      total++;
    }
  }
  return total;
}

int main(void) {
  printf("%d %d\n", count_null_pairs(), count_true_flags());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     value: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn count_null_pairs() -> i32 {
// CHECK-NEXT:     let pairs: __SlateAlign16<[pair; 1]> = __SlateAlign16([
// CHECK-NEXT:         pair {
// CHECK-NEXT:             name: std::ptr::null::<i8>(),
// CHECK-NEXT:             value: std::ptr::null::<i8>(),
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..1 as i32 {
// CHECK-NEXT:         if pairs.0[i as usize].name == std::ptr::null::<i8>()
// CHECK-NEXT:             && pairs.0[i as usize].value == std::ptr::null::<i8>()
// CHECK-NEXT:         {
// CHECK-NEXT:             total += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn count_true_flags() -> i32 {
// CHECK-NEXT:     let values: [bool; 2] = [true as bool, false as bool];
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..((2 as u64) / (1 as u64)) {
// CHECK-NEXT:         if values[i as usize] {
// CHECK-NEXT:             total += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", count_null_pairs(), count_true_flags());
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
