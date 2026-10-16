/* Originally added to test SH constant pool layout.  t1() failed for
   non-PIC and t2() failed for PIC.  */

void abort(void);
void exit(int);

int t1(float *f, int i, void (*f1)(double), void (*f2)(float, float)) {
  f1(3.0);
  f[i] = f[i + 1];
  f2(2.5f, 3.5f);
}

int t2(float *f, int i, void (*f1)(double), void (*f2)(float, float),
       void (*f3)(float)) {
  f3(6.0f);
  f1(3.0);
  f[i] = f[i + 1];
  f2(2.5f, 3.5f);
}

void f1(double d) {
  if (d != 3.0)
    abort();
}

void f2(float f1, float f2) {
  if (f1 != 2.5f || f2 != 3.5f)
    abort();
}

void f3(float f) {
  if (f != 6.0f)
    abort();
}

int main() {
  float f[3] = {2.0f, 3.0f, 4.0f};
  t1(f, 0, f1, f2);
  t2(f, 1, f1, f2, f3);
  if (f[0] != 3.0f && f[1] != 4.0f)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Originally added to test SH constant pool layout.  t1() failed for
// CHECK-NEXT: //! non-PIC and t2() failed for PIC.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn t1(
// CHECK-NEXT:     mut f: *mut f32,
// CHECK-NEXT:     mut i: i32,
// CHECK-NEXT:     mut f1: Option<unsafe extern "C-unwind" fn(f64)>,
// CHECK-NEXT:     mut f2: Option<unsafe extern "C-unwind" fn(f32, f32)>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { f1.unwrap()(3.0f64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { f.offset(i as isize) } =
// CHECK-NEXT:             unsafe { *unsafe { f.offset((i + (1 as i32)) as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f2.unwrap()(2.5f32, 3.5f32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn t2(
// CHECK-NEXT:     mut f: *mut f32,
// CHECK-NEXT:     mut i: i32,
// CHECK-NEXT:     mut f1: Option<unsafe extern "C-unwind" fn(f64)>,
// CHECK-NEXT:     mut f2: Option<unsafe extern "C-unwind" fn(f32, f32)>,
// CHECK-NEXT:     mut f3: Option<unsafe extern "C-unwind" fn(f32)>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { f3.unwrap()(6.0f32) };
// CHECK-NEXT:     unsafe { f1.unwrap()(3.0f64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { f.offset(i as isize) } =
// CHECK-NEXT:             unsafe { *unsafe { f.offset((i + (1 as i32)) as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f2.unwrap()(2.5f32, 3.5f32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn f1(mut d: f64) {
// CHECK-NEXT:     if d != 3.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn f2(mut f1: f32, mut f2: f32) {
// CHECK-NEXT:     if f1 != 2.5f32 || f2 != 3.5f32 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn f3(mut f: f32) {
// CHECK-NEXT:     if f != 6.0f32 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: [f32; 3] = [2.0f32, 3.0f32, 4.0f32];
// CHECK-NEXT:     t1(f.as_mut_ptr() as *mut f32, 0 as i32, Some(f1), Some(f2));
// CHECK-NEXT:     t2(
// CHECK-NEXT:         f.as_mut_ptr() as *mut f32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         Some(f1),
// CHECK-NEXT:         Some(f2),
// CHECK-NEXT:         Some(f3),
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { *unsafe { (f.as_mut_ptr() as *mut f32).offset((0 as i32) as isize) } }) != 3.0f32
// CHECK-NEXT:         && (unsafe { *unsafe { (f.as_mut_ptr() as *mut f32).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != 4.0f32
// CHECK-NEXT:     {
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
