/* On the i960 any arg bigger than 16 bytes causes all subsequent args
   to be passed on the stack.  We test this.  */

#include <stdarg.h>

void abort(void);
void exit(int);

typedef struct {
  char a[32];
} big;

void f(big x, char *s, ...) {
  va_list ap;

  if (x.a[0] != 'a' || x.a[1] != 'b' || x.a[2] != 'c')
    abort();
  va_start(ap, s);
  if (va_arg(ap, int) != 42)
    abort();
  if (va_arg(ap, int) != 'x')
    abort();
  if (va_arg(ap, int) != 0)
    abort();
  va_end(ap);
}

int main(void) {
  static big x = {"abc"};

  f(x, "", 42, 'x', 0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! On the i960 any arg bigger than 16 bytes causes all subsequent args
// CHECK-NEXT: //! to be passed on the stack.  We test this.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: [i8; 32],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f(mut [[X_0:x_[0-9]+]]: [[SLATERECORD_0]], mut s: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if ((unsafe { *unsafe { ([[X_0]].a.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } }) as i32)
// CHECK-NEXT:         != (97 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { ([[X_0]].a.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (98 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { ([[X_0]].a.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (99 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (42 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (120 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f(
// CHECK-NEXT:             unsafe { x },
// CHECK-NEXT:             c"".as_ptr() as *mut i8,
// CHECK-NEXT:             42 as i32,
// CHECK-NEXT:             120 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     a: [
// CHECK-NEXT:         97 as i8, 98 as i8, 99 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:     ],
// CHECK-NEXT: };
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
