extern void abort(void);

void __attribute__((noinline)) bar(int **p) {
  float *q = (float *)p;
  *q       = 0.0;
}

float __attribute__((noinline)) foo(int b) {
  int  *i = 0;
  float f = 1.0;
  int **p;
  if (b)
    p = &i;
  else
    p = (int **)&f;
  bar(p);
  if (b)
    return **p;
  return f;
}

int main() {
  if (foo(0) != 0.0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut p: *mut *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut f32) = 0.0f64 as f32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut b: i32) -> f32 {
// CHECK-NEXT:     let mut i: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     let mut f: f32 = 1.0f64 as f32;
// CHECK-NEXT:     let mut p: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         p = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         p = std::ptr::addr_of_mut!(f) as *mut *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(p);
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return (unsafe { *unsafe { *p } }) as f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (foo(0 as i32) as f64) != 0.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
