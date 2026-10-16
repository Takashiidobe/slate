int         flag;
extern void abort(void);
int         main() {
  int x;
  if (flag)
    x = -1;
  else
    x &= 0xff;
  if (x & ~0xff)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut flag: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     if (unsafe { flag }) != (0 as i32) {
// CHECK-NEXT:         x = -(1 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] & (255 as i32);
// CHECK-NEXT:         x = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if x & !(255 as i32) != (0 as i32) {
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
