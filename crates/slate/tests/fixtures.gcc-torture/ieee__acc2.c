/* { dg-do run } */
/* Tail call optimizations would reverse the order of multiplications
   in func().  */

void abort(void);
void exit(int);

double func(const double *array) {
  double d = *array;
  if (d == 1.0)
    return d;
  else
    return d * func(array + 1);
}

int main() {
  double values[] = {__DBL_MAX__, 2.0, 0.5, 1.0};
  if (func(values) != __DBL_MAX__)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Tail call optimizations would reverse the order of multiplications
// CHECK-NEXT: //! in func().
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn func(mut array: *const f64) -> f64 {
// CHECK-NEXT:     let mut d: f64 = unsafe { *array };
// CHECK-NEXT:     if d == 1.0f64 {
// CHECK-NEXT:         return d;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return d * func(unsafe { array.offset((1 as i32) as isize) });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[f64; 4]> =
// CHECK-NEXT:         __SlateAlign16([1.7976931348623157e308f64, 2.0f64, 0.5f64, 1.0f64]);
// CHECK-NEXT:     if func(values.0.as_mut_ptr() as *const f64) != 1.7976931348623157e308f64 {
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
