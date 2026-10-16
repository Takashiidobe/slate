/* { dg-do run } */
/* Tail call optimizations would convert func() into the moral equivalent of:

       double acc = 0.0;
       for (int i = 0; i <= n; i++)
         acc += d;
       return acc;

   which mishandles the case where 'd' is -0.  They also initialised 'acc'
   to a zero int rather than a zero double.  */

void abort(void);
void exit(int);

double func(double d, int n) {
  if (n == 0)
    return d;
  else
    return d + func(d, n - 1);
}

int main() {
  if (__builtin_copysign(1.0, func(0.0 / -5.0, 10)) != -1.0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Tail call optimizations would convert func() into the moral equivalent of:
// CHECK-NEXT: //!
// CHECK-NEXT: //!     double acc = 0.0;
// CHECK-NEXT: //!     for (int i = 0; i <= n; i++)
// CHECK-NEXT: //!       acc += d;
// CHECK-NEXT: //!     return acc;
// CHECK-NEXT: //!
// CHECK-NEXT: //! which mishandles the case where 'd' is -0.  They also initialised 'acc'
// CHECK-NEXT: //! to a zero int rather than a zero double.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.copysign.f64"]
// CHECK-NEXT:     fn __slate_intrinsic_90b7711c6b7522c7([[ARG_0:arg[0-9]+]]: f64, [[ARG_1:arg[0-9]+]]: f64) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn func(mut d: f64, mut n: i32) -> f64 {
// CHECK-NEXT:     if n == (0 as i32) {
// CHECK-NEXT:         return d;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return d + func(d, n - (1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { __slate_intrinsic_90b7711c6b7522c7(1.0f64, func(0.0f64 / -5.0f64, 10 as i32)) })
// CHECK-NEXT:         != -1.0f64
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
