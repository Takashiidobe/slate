#include <stdarg.h>

void abort(void);
void exit(int);

/*typedef unsigned long L;*/
typedef double L;
void           f(L p0, L p1, L p2, L p3, L p4, L p5, L p6, L p7, L p8, ...) {
  va_list select;

  va_start(select, p8);

  if (va_arg(select, L) != 10.)
    abort();
  if (va_arg(select, L) != 11.)
    abort();
  if (va_arg(select, L) != 0.)
    abort();

  va_end(select);
}

int main() {
  f(1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 0.);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// typedef unsigned long L;
// CHECK-NEXT: unsafe extern "C-unwind" fn f(
// CHECK-NEXT:     mut p0: f64,
// CHECK-NEXT:     mut p1: f64,
// CHECK-NEXT:     mut p2: f64,
// CHECK-NEXT:     mut p3: f64,
// CHECK-NEXT:     mut p4: f64,
// CHECK-NEXT:     mut p5: f64,
// CHECK-NEXT:     mut p6: f64,
// CHECK-NEXT:     mut p7: f64,
// CHECK-NEXT:     mut p8: f64,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut select: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     select = __va_args.clone();
// CHECK-NEXT:     if (unsafe { select.next_arg::<f64>() }) != 10.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { select.next_arg::<f64>() }) != 11.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { select.next_arg::<f64>() }) != 0.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f(
// CHECK-NEXT:             1.0f64, 2.0f64, 3.0f64, 4.0f64, 5.0f64, 6.0f64, 7.0f64, 8.0f64, 9.0f64, 10.0f64,
// CHECK-NEXT:             11.0f64, 0.0f64,
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
