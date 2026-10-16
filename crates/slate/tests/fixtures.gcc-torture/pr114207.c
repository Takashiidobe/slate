#include <stdint.h>
#include <stdio.h>

struct S {
  int a, b;
};

__attribute__((noinline)) void foo(struct S *s) {
  struct S ss = (struct S){.a = s->b, .b = s->a};
  *s          = ss;
}

int main() {
  struct S s = {6, 12};
  foo(&s);
  if (s.a != 12 || s.b != 6)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut s: &mut S) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ss: S = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = S {
// CHECK-NEXT:                 a: unsafe { (*(s as *mut S)).b },
// CHECK-NEXT:                 b: unsafe { (*(s as *mut S)).a },
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(s as *mut S) = ss;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = S { a: 6 as i32, b: 12 as i32 };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if s.a != (12 as i32) || s.b != (6 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
