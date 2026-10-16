void abort(void);
void exit(int);

struct complex {
  float r;
  float i;
};

struct complex cmplx(float, float);

struct complex f(float a, float b) {
  struct complex c;
  c.r = a;
  c.i = b;
  return c;
}

int main(void) {
  struct complex z = f(1.0, 0.0);

  if (z.r != 1.0 || z.i != 0.0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn cmplx([[V_1:__v[0-9]+]]: f32, [[V_2:__v[0-9]+]]: f32) -> complex;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct complex {
// CHECK-NEXT:     r: f32,
// CHECK-NEXT:     i: f32,
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: f32, mut b: f32) -> complex {
// CHECK-NEXT:     let mut c: complex = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     c.r = a;
// CHECK-NEXT:     c.i = b;
// CHECK-NEXT:     return c;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut z: complex = f(1.0f64 as f32, 0.0f64 as f32);
// CHECK-NEXT:     if (z.r as f64) != 1.0f64 || (z.i as f64) != 0.0f64 {
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
