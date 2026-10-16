/* Copyright (C) 2000  Free Software Foundation.

   If the argument to va_end() has side effects, test whether side
   effects from that argument are honored.

   Written by Kaveh R. Ghazi, 10/31/2000.  */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#define __attribute__(x)
#endif

static void __attribute__((__format__(__printf__, 1, 2))) doit(const char *s,
                                                               ...) {
  va_list *ap_array[3], **ap_ptr = ap_array;

  ap_array[0] = malloc(sizeof(va_list));
  ap_array[1] = NULL;
  ap_array[2] = malloc(sizeof(va_list));

  va_start(*ap_array[0], s);
  vprintf(s, **ap_ptr);
  /* Increment the va_list pointer once.  */
  va_end(**ap_ptr++);

  /* Increment the va_list pointer a second time.  */
  ap_ptr++;

  va_start(*ap_array[2], s);
  /* If we failed to increment ap_ptr twice, then the parameter passed
     in here will dereference NULL and should cause a crash.  */
  vprintf(s, **ap_ptr);
  va_end(**ap_ptr);

  /* Just in case, If *ap_ptr is NULL abort anyway.  */
  if (*ap_ptr == 0)
    abort();
}

int main() {
  doit("%s", "hello world\n");
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! If the argument to va_end() has side effects, test whether side
// CHECK-NEXT: //! effects from that argument are honored.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Kaveh R. Ghazi, 10/31/2000.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vprintf(__format: *const i8, __arg: core::ffi::VaList<'_>) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn doit(mut s: *const i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap_array: __SlateAlign16<[*mut core::ffi::VaList<'_>; 3]> =
// CHECK-NEXT:         __SlateAlign16([0 as *mut core::ffi::VaList<'_>; 3]);
// CHECK-NEXT:     let mut ap_ptr: *mut *mut core::ffi::VaList<'_> =
// CHECK-NEXT:         ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>).offset((0 as i32) as isize)
// CHECK-NEXT:         } = (unsafe { malloc(24 as u64) }) as *mut core::ffi::VaList<'_>;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>).offset((1 as i32) as isize)
// CHECK-NEXT:         } = std::ptr::null_mut::<core::ffi::VaList<'_>>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>).offset((2 as i32) as isize)
// CHECK-NEXT:         } = (unsafe { malloc(24 as u64) }) as *mut core::ffi::VaList<'_>;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 (ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>)
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } = __va_args.clone();
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vprintf(s, unsafe { (*unsafe { *ap_ptr }).clone() }) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     // Increment the va_list pointer once.
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut *mut core::ffi::VaList<'_> = ap_ptr;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut *mut core::ffi::VaList<'_> = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:     ap_ptr = [[V_1]];
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Increment the va_list pointer a second time.
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut *mut core::ffi::VaList<'_> = ap_ptr;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut *mut core::ffi::VaList<'_> = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     ap_ptr = [[V_3]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 (ap_array.0.as_mut_ptr() as *mut *mut core::ffi::VaList<'_>)
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } = __va_args.clone();
// CHECK-NEXT:     }
// CHECK-NEXT:     // If we failed to increment ap_ptr twice, then the parameter passed
// CHECK-NEXT:     // in here will dereference NULL and should cause a crash.
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vprintf(s, unsafe { (*unsafe { *ap_ptr }).clone() }) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Just in case, If *ap_ptr is NULL abort anyway.
// CHECK-NEXT:     if (unsafe { *ap_ptr }) == std::ptr::null_mut::<core::ffi::VaList<'_>>() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         doit(
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello world\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
