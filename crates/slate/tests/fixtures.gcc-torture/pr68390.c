/* { dg-do run }  */
/* { dg-options "-O2" } */

__attribute__((noinline)) double direct(int x, ...) { return x * x; }

__attribute__((noinline)) double broken(double (*indirect)(int x, ...), int v) {
  return indirect(v);
}

int main() {
  double d1, d2;
  int    i = 2;
  d1       = broken(direct, i);
  if (d1 != i * i) {
    __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn direct(mut x: i32, mut __va_args: ...) -> f64 {
// CHECK-NEXT:     (x * x) as f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn broken(mut indirect: Option<unsafe extern "C-unwind" fn(i32, ...) -> f64>, mut v: i32) -> f64 {
// CHECK-NEXT:     unsafe { indirect.unwrap()(v) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d1: f64 = 0 as f64;
// CHECK-NEXT:     let mut i: i32 = 2 as i32;
// CHECK-NEXT:     d1 = broken(Some(direct), i);
// CHECK-NEXT:     if d1 != ((i * i) as f64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
