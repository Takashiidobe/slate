#include <stddef.h>
#include <stdlib.h>

struct FlexibleArray {
  size_t count;
  int    values[];
};

int main(void) {
  if (sizeof(struct FlexibleArray) != sizeof(size_t)) {
    return 1;
  }

  struct FlexibleArray *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }

  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
  }

  int total = 0;
  for (size_t index = 0; index < flexible->count; ++index) {
    total += flexible->values[index];
  }

  free(flexible);
  return total == 6 ? 0 : 3;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct FlexibleArray {
// CHECK-NEXT:     count: u64,
// CHECK-NEXT:     values: [i32; 0],
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if 8u64 != 8u64 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let flexible: *mut FlexibleArray = (unsafe { malloc(20u64) }) as *mut FlexibleArray;
// CHECK-NEXT:     if flexible.is_null() {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flexible).count = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut index: u64 = 0;
// CHECK-NEXT:         while index < unsafe { (*flexible).count } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(std::ptr::addr_of_mut!((* flexible).values) as *mut i32)
// CHECK-NEXT:                     .offset(index as isize) = ((index as u32) as i32) + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             index = index.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut index: u64 = 0;
// CHECK-NEXT:     while index < unsafe { (*flexible).count } {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = total
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *(std::ptr::addr_of_mut!((* flexible).values) as *mut i32)
// CHECK-NEXT:                     .offset(index as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:         total = [[V_0]];
// CHECK-NEXT:         index = index.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(flexible as *mut ()) };
// CHECK-NEXT:     if total == 6 { 0 } else { 3 }
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
