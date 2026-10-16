/* Copyright (C) 2000  Free Software Foundation  */
/* by Alexandre Oliva <aoliva@redhat.com> */

#include <stdlib.h>

void bar(int);
void foo(int *);

int main() {
  static int a[] = {0, 1, 2};
  int       *i   = &a[sizeof(a) / sizeof(*a)];

  while (i-- > a)
    foo(i);

  exit(0);
}

void baz(int, int);

void bar(int i) { baz(i, i); }
void foo(int *i) { bar(*i); }

void baz(int i, int j) {
  if (i != j)
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation
// CHECK-NEXT: //!
// CHECK-NEXT: //! by Alexandre Oliva <aoliva@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: *mut i32 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(((12 as u64) / (4 as u64)) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i32 = unsafe { [[V_0]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:         if !([[V_0]] > unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         foo(i);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [i32; 3] = [0 as i32, 1 as i32, 2 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut i: i32) {
// CHECK-NEXT:     baz(i, i);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: *mut i32) {
// CHECK-NEXT:     bar(unsafe { *i });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut i: i32, mut j: i32) {
// CHECK-NEXT:     if i != j {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
