#include <stdarg.h>

void abort(void);
void exit(int);

typedef int TYPE;

void vafunction(char *dummy, ...) {
  va_list ap;

  va_start(ap, dummy);
  if (va_arg(ap, TYPE) != 1)
    abort();
  if (va_arg(ap, TYPE) != 2)
    abort();
  if (va_arg(ap, TYPE) != 3)
    abort();
  if (va_arg(ap, TYPE) != 4)
    abort();
  if (va_arg(ap, TYPE) != 5)
    abort();
  if (va_arg(ap, TYPE) != 6)
    abort();
  if (va_arg(ap, TYPE) != 7)
    abort();
  if (va_arg(ap, TYPE) != 8)
    abort();
  if (va_arg(ap, TYPE) != 9)
    abort();
  va_end(ap);
}

int main(void) {
  vafunction("", 1, 2, 3, 4, 5, 6, 7, 8, 9);
  exit(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn vafunction(mut dummy: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (6 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ap.next_arg::<i32>() }) != (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         vafunction(
// CHECK-NEXT:             c"".as_ptr() as *mut i8,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
