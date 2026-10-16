void abort(void);
void exit(int);

int a = 1;
int b = -1;

int c = 1;
int d = 0;

int main(void) {
  double e;
  double f;
  double g;

  f = c;
  g = d;
  e = (a < b) ? f : g;
  if (e)
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
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-NEXT: static mut b: i32 = -(1 as i32);
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-NEXT: static mut d: i32 = 0 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: f64 = 0 as f64;
// CHECK-NEXT:     let mut f: f64 = (unsafe { c }) as f64;
// CHECK-NEXT:     let mut g: f64 = (unsafe { d }) as f64;
// CHECK-NEXT:     e = if (unsafe { a }) < unsafe { b } { f } else { g };
// CHECK-NEXT:     if e != 0.0f64 {
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
