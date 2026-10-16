void abort(void);
void exit(int);

int main() {
  double x = 1.0;
  double y = 2.0;

  if ((y > x--) != 1)
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f64 = 1.0f64;
// CHECK-NEXT:     let mut y: f64 = 2.0f64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: f64 = x;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: f64 = [[V_1]] - ((1 as i32) as f64);
// CHECK-NEXT:     x = [[V_2]];
// CHECK-NEXT:     if ((y > [[V_1]]) as i32) != (1 as i32) {
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
