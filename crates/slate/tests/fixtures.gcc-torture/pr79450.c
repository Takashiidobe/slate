/* PR rtl-optimization/79450 */

unsigned int foo(unsigned char x, unsigned long long y) {
  do {
    x &= !y;
    x %= 24;
  } while (x < y);
  return x + y;
}

int main(void) {
  unsigned int x = foo(1, 0);
  if (x != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/79450
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u8, mut y: u64) -> u32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u8 = x;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u8 = (((([[V_0]] as u32) as i32) & (!(y != (0 as u64)) as i32)) as i8) as u8;
// CHECK-NEXT:         x = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u8 = x;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u8 = (((([[V_2]] as u32) as i32) % (24 as i32)) as i8) as u8;
// CHECK-NEXT:         x = [[V_3]];
// CHECK-NEXT:         if !(((((x as u32) as i32) as i64) as u64) < y) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     ((((x as u32) as i32) as i64) as u64).wrapping_add(y) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = foo(((1 as i32) as i8) as u8, ((0 as i32) as i64) as u64);
// CHECK-NEXT:     if x != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
