void abort(void);
void exit(int);

double test(const double le, const double ri) {
  double val = (ri - le) / (ri * (le + 1.0));
  return val;
}

int main() {
  double retval;

  retval = test(1.0, 2.0);
  if (retval < 0.24 || retval > 0.26)
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
// CHECK-NEXT: fn test(mut le: f64, mut ri: f64) -> f64 {
// CHECK-NEXT:     let mut val: f64 = (ri - le) / (ri * (le + 1.0f64));
// CHECK-NEXT:     val
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut retval: f64 = test(1.0f64, 2.0f64);
// CHECK-NEXT:     if retval < 0.24f64 || retval > 0.26f64 {
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
