/* PR tree-optimization/81913 */

typedef __UINT8_TYPE__  u8;
typedef __UINT32_TYPE__ u32;

static u32 b(u8 d, u32 e, u32 g) {
  do {
    e += g + 1;
    d--;
  } while (d >= (u8)e);

  return e;
}

int main(void) {
  u32 x = b(1, -0x378704, ~0xba64fc);
  if (x != 0xd93190d0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/81913
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn b(mut d: u8, mut e: u32, mut g: u32) -> u32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = e;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add(g.wrapping_add((1 as i32) as u32));
// CHECK-NEXT:         e = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u8 = d;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u8 = (((([[V_2]] as u32) as i32) - (1 as i32)) as i8) as u8;
// CHECK-NEXT:         d = [[V_3]];
// CHECK-NEXT:         if !(((d as u32) as i32) >= (((e as u8) as u32) as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     e
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = b(
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         -(3639044 as i32) as u32,
// CHECK-NEXT:         !(12215548 as i32) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if x != (3643904208 as u32) {
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
