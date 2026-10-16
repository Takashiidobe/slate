/* { dg-do run } */
void abort(void);
void exit(int);

unsigned u  = 2147483839;
float    f0 = 2147483648e0, f1 = 2147483904e0;
int      main(void) {
  float f = u;
  if (f == f0)
    abort();
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
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: static mut u: u32 = ((2147483839 as i64) as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f0: f32 = 2147483648.0f64 as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f1: f32 = 2147483904.0f64 as f32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: f32 = (unsafe { u }) as f32;
// CHECK-NEXT:     if f == unsafe { f0 } {
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
