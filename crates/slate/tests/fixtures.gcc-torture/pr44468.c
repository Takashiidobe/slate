#include <stddef.h>

struct S {
  int i;
  int j;
};
struct R {
  int      k;
  struct S a;
};
struct Q {
  float    k;
  struct S a;
};
struct Q                               s;
int __attribute__((noinline, noclone)) test1(void *q) {
  struct S *b = (struct S *)((char *)q + sizeof(int));
  s.a.i       = 0;
  b->i        = 3;
  return s.a.i;
}
int __attribute__((noinline, noclone)) test2(void *q) {
  struct S *b = &((struct R *)q)->a;
  s.a.i       = 0;
  b->i        = 3;
  return s.a.i;
}
int __attribute__((noinline, noclone)) test3(void *q) {
  s.a.i                                      = 0;
  ((struct S *)((char *)q + sizeof(int)))->i = 3;
  return s.a.i;
}
extern void abort(void);
int         main() {
  if (sizeof(float) != sizeof(int) || offsetof(struct R, a) != sizeof(int) ||
      offsetof(struct Q, a) != sizeof(int))
    return 0;
  s.a.i = 1;
  s.a.j = 2;
  if (test1((void *)&s) != 3)
    abort();
  s.a.i = 1;
  s.a.j = 2;
  if (test2((void *)&s) != 3)
    abort();
  s.a.i = 1;
  s.a.j = 2;
  if (test3((void *)&s) != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct R {
// CHECK-NEXT:     k: i32,
// CHECK-NEXT:     a: S,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Q {
// CHECK-NEXT:     k: f32,
// CHECK-NEXT:     a: S,
// CHECK-NEXT: }
// CHECK-NEXT: static mut s: Q = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn test1(mut q: *mut ()) -> i32 {
// CHECK-NEXT:     let mut b: *mut S = (unsafe { (q as *mut i8).offset((4 as u64) as isize) })
// CHECK-NEXT:         as *mut S;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*b).i = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { s.a.i }
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut q: *mut ()) -> i32 {
// CHECK-NEXT:     let mut b: *mut S = unsafe { std::ptr::addr_of_mut!((* (q as * mut R)).a) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*b).i = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { s.a.i }
// CHECK-NEXT: }
// CHECK-NEXT: fn test3(mut q: *mut ()) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*((unsafe { (q as *mut i8).offset((4 as u64) as isize) }) as *mut S)).i = 3
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { s.a.i }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64) != (4 as u64) || (4 as u64) != (4 as u64) || (4 as u64) != (4 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.j = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1((unsafe { std::ptr::addr_of_mut!(s) }) as *mut ()) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.j = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2((unsafe { std::ptr::addr_of_mut!(s) }) as *mut ()) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.a.j = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3((unsafe { std::ptr::addr_of_mut!(s) }) as *mut ()) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
