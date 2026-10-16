#include <limits.h>

void abort(void);
void exit(int);

int a[2];

void f(int i) {
  for (; i < INT_MAX; i++) {
    a[i] = -2;
    if (&a[i] == &a[1])
      break;
  }
}

int main(void) {
  a[0] = a[1] = 0;
  f(0);
  if (a[0] != -2 || a[1] != -2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i32; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f(mut i: i32) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2147483647 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             } = -(2 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             == unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f(0 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != -(2 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != -(2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
