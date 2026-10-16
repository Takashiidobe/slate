/* Verify that the CALL sideeffect isn't optimized away.  */
/* Contributed by Greg Parker  25 Jan 2005  <gparker@apple.com> */

#include <stdio.h>
#include <stdlib.h>

struct parse {
  char *next;
  char *end;
  int   error;
};

int seterr(struct parse *p, int err) {
  p->error = err;
  return 0;
}

void bracket_empty(struct parse *p) {
  if (((p->next < p->end) && (*p->next++) == ']') || seterr(p, 7)) {
  }
}

int main(int    argc __attribute__((unused)),
         char **argv __attribute__((unused))) {
  struct parse p;
  p.next = p.end = (char *)0x12345;

  p.error = 0;
  bracket_empty(&p);
  if (p.error != 7)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Verify that the CALL sideeffect isn't optimized away.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Contributed by Greg Parker  25 Jan 2005  <gparker@apple.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct parse {
// CHECK-NEXT:     next: *mut i8,
// CHECK-NEXT:     end: *mut i8,
// CHECK-NEXT:     error: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn seterr(mut p: &mut parse, mut err: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut parse)).error = err;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bracket_empty(mut p: &mut parse) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { (*(p as *mut parse)).next }) < unsafe { (*(p as *mut parse)).end } {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { (*p).next };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*p).next = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = ((unsafe { *[[V_1]] }) as i32) == (93 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = seterr(p, 7 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_3]] {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut p: parse = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p.end = (74565 as i32) as *mut i8;
// CHECK-NEXT:     p.next = (74565 as i32) as *mut i8;
// CHECK-NEXT:     p.error = 0 as i32;
// CHECK-NEXT:     bracket_empty(unsafe { &mut (*std::ptr::addr_of_mut!(p)) });
// CHECK-NEXT:     if p.error != (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
