

int   a = 1, b, d;
short e;

int main() {
  for (; b; b++)
    ;
  short f = a;
  int   g = 15;
  e       = f ? f : 1 << g;
  int h   = e;
  d       = h == 83647 ? 0 : h;
  if (d != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut f: i16 = (unsafe { a }) as i16;
// CHECK-NEXT:     let mut g: i32 = 15 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = (if f != (0 as i16) { f as i32 } else { (1 as i32) << g }) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut h: i32 = (unsafe { e }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = if h == (83647 as i32) { 0 as i32 } else { h };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
