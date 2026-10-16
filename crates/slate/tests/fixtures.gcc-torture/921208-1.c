void abort(void);
void exit(int);

double f(double x) { return x * x; }

double Int(double (*f)(double), double a) { return (*f)(a); }

int main(void) {
  if (Int(&f, 2.0) != 4.0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn f(mut x: f64) -> f64 {
// CHECK-NEXT:     x * x
// CHECK-NEXT: }
// CHECK-NEXT: fn Int(mut f: Option<unsafe extern "C-unwind" fn(f64) -> f64>, mut a: f64) -> f64 {
// CHECK-NEXT:     unsafe { f.unwrap()(a) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if Int(Some(f), 2.0f64) != 4.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
