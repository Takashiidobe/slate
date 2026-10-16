void abort(void);
void exit(int);

typedef struct {
  unsigned short b0, b1, b2, b3;
} four_quarters;

four_quarters x;
int           a, b;

void f(four_quarters j) {
  b = j.b2;
  a = j.b3;
}

int main(void) {
  four_quarters x;
  x.b0 = x.b1 = x.b2 = 0;
  x.b3               = 38;
  f(x);
  if (a != 38)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     b0: u16,
// CHECK-NEXT:     b1: u16,
// CHECK-NEXT:     b2: u16,
// CHECK-NEXT:     b3: u16,
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f(mut j: [[SLATERECORD_0]]) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (j.b2 as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (j.b3 as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[X_0:x_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[X_0]].b2 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     [[X_0]].b1 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     [[X_0]].b0 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     [[X_0]].b3 = ((38 as i32) as i16) as u16;
// CHECK-NEXT:     f([[X_0]]);
// CHECK-NEXT:     if (unsafe { a }) != (38 as i32) {
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
