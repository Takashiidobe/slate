void abort(void);
void exit(int);

void f(double *ty) { *ty = -1.0; }

int main(void) {
  double foo[6];
  double tx = 0.0, ty, d;

  f(&ty);

  if (ty < 0)
    ty = -ty;
  d = (tx > ty) ? tx : ty;
  if (ty != d)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn f(mut ty: &mut f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(ty as *mut f64) = -1.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     __SlateAlign16([0 as f64; 6]);
// CHECK-NEXT:     let mut tx: f64 = 0.0f64;
// CHECK-NEXT:     let mut ty: f64 = 0 as f64;
// CHECK-NEXT:     let mut d: f64 = 0 as f64;
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(ty)) });
// CHECK-NEXT:     if ty < ((0 as i32) as f64) {
// CHECK-NEXT:         ty = -ty;
// CHECK-NEXT:     }
// CHECK-NEXT:     d = if tx > ty { tx } else { ty };
// CHECK-NEXT:     if ty != d {
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
