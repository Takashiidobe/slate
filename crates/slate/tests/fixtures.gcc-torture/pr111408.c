/* PR target/111408 */

int   a, b, c, d;
short e;

int foo() {
  c = a % (sizeof(int) * 8);
  if (b & 1 << c)
    return -1;
  return 0;
}

int main() {
  for (; e != 1; e++) {
    int g = foo();
    if (g + d - 9 + d)
      continue;
    for (;;)
      __builtin_abort();
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/111408
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = (((((unsafe { a }) as i64) as u64)
// CHECK-NEXT:             % (4 as u64).wrapping_mul(((8 as i32) as i64) as u64)) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) & (1 as i32) << unsafe { c } != (0 as i32) {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { e }) as i32) != (1 as i32) {
// CHECK-NEXT:         let mut g: i32 = foo();
// CHECK-NEXT:         if g + unsafe { d } - (9 as i32) + unsafe { d } != (0 as i32) {
// CHECK-NEXT:         } else {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = unsafe { e };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) + (1 as i32)) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
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
