/* Copyright (C) 2003  Free Software Foundation.

   Test strcpy optimizations don't evaluate side-effects twice.

   Written by Jakub Jelinek, June 23, 2003.  */

typedef __SIZE_TYPE__ size_t;
extern char          *strcpy(char *, const char *);
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);
extern void           exit(int);

size_t test1(char *s, size_t i) {
  strcpy(s, "foobarbaz" + i++);
  return i;
}

size_t check2(void) {
  static size_t r = 5;
  if (r != 5)
    abort();
  return ++r;
}

void test2(char *s) { strcpy(s, "foobarbaz" + check2()); }

int main(void) {
  char buf[10];
  if (test1(buf, 7) != 8 || memcmp(buf, "az", 3))
    abort();
  test2(buf);
  if (memcmp(buf, "baz", 4))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2003  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test strcpy optimizations don't evaluate side-effects twice.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Jakub Jelinek, June 23, 2003.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strcpy([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn memcmp([[V_2:__v[0-9]+]]: *const (), [[V_3:__v[0-9]+]]: *const (), [[V_4:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_5:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut s: *mut i8, mut i: u64) -> u64 {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u64 = [[V_6]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:     i = [[V_7]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             s,
// CHECK-NEXT:             (unsafe { (c"foobarbaz".as_ptr() as *mut i8).offset([[V_6]] as isize) }) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check2() -> u64 {
// CHECK-NEXT:     if (unsafe { r }) != (((5 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u64 = unsafe { r };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u64 = [[V_8]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         r = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_9]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: u64 = ((5 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut s: *mut i8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             s,
// CHECK-NEXT:             (unsafe { (c"foobarbaz".as_ptr() as *mut i8).offset(check2() as isize) }) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if test1(buf.as_mut_ptr() as *mut i8, ((7 as i32) as i64) as u64)
// CHECK-NEXT:         != (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 buf.as_mut_ptr() as *const (),
// CHECK-NEXT:                 b"az\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((3 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     test2(buf.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             buf.as_mut_ptr() as *const (),
// CHECK-NEXT:             b"baz\0".as_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
