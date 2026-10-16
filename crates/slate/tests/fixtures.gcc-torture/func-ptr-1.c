void abort(void);
void exit(int);

static double f(float a);
static double (*fp)(float a);

int main(void) {
  fp = f;
  if (fp((float)1) != 1.0)
    abort();
  exit(0);
}

static double f(float a) { return a; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut fp: Option<unsafe extern "C-unwind" fn(f32) -> f64> = unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fp = Some(f);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { unsafe { fp }.unwrap()((1 as i32) as f32) }) != 1.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn f(mut a: f32) -> f64 {
// CHECK-NEXT:     a as f64
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
