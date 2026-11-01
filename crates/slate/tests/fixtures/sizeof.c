#include <stdio.h>

struct Pair {
  int left;
  int right;
};

union Slot {
  int          value;
  unsigned int raw;
};

static int sizeof_values(void) {
  int values[3];
  return sizeof(int) + sizeof(values) + sizeof(struct Pair) +
         sizeof(union Slot);
}

static int sizeof_expr(void) {
  int value = 0;
  return sizeof value;
}

int main(void) {
  printf("%d\n", sizeof_values());
  printf("%d\n", sizeof_expr());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Slot {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     raw: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn sizeof_values() -> i32 {
// CHECK-NEXT:     ((std::mem::size_of::<i32>() as u64)
// CHECK-NEXT:         .wrapping_add(std::mem::size_of::<[i32; 3]>() as u64)
// CHECK-NEXT:         .wrapping_add(std::mem::size_of::<Pair>() as u64)
// CHECK-NEXT:         .wrapping_add(std::mem::size_of::<Slot>() as u64) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn sizeof_expr() -> i32 {
// CHECK-NEXT:     std::mem::size_of::<i32>() as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sizeof_values());
// CHECK-NEXT:     println!("{}", sizeof_expr());
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
