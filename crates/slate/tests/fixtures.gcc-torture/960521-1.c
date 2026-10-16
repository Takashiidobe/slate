/* { dg-add-options stack_size } */

#include <stdlib.h>

int *a, *b;
int  n;

#ifdef STACK_SIZE
#define BLOCK_SIZE (STACK_SIZE / (sizeof(*a) + sizeof(*b)))
#else
#define BLOCK_SIZE 32768
#endif
void foo(void) {
  int i;
  for (i = 0; i < n; i++)
    a[i] = -1;
  for (i = 0; i < BLOCK_SIZE - 1; i++)
    b[i] = -1;
}

int main(void) {
  n    = BLOCK_SIZE;
  a    = malloc(n * sizeof(*a));
  b    = malloc(n * sizeof(*b));
  *b++ = 0;
  foo();
  if (b[-1])
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < unsafe { n } {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a }.offset(i as isize) } = -(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (32768 as i32) - (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { b }.offset(i as isize) } = -(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         n = 32768 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (unsafe { malloc((((unsafe { n }) as i64) as u64).wrapping_mul(4 as u64)) })
// CHECK-NEXT:             as *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (unsafe { malloc((((unsafe { n }) as i64) as u64).wrapping_mul(4 as u64)) })
// CHECK-NEXT:             as *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i32 = unsafe { b };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: *mut i32 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { b }.offset(-(1 as i32) as isize) } }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
