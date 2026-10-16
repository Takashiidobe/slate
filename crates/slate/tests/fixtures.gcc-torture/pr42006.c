extern void abort(void);

static unsigned int my_add(unsigned int si1, unsigned int si2) {
  return (si1 > (50 - si2)) ? si1 : (si1 + si2);
}

static unsigned int my_shift(unsigned int left, unsigned int right) {
  return (right > 100) ? left : (left >> right);
}

static int func_4(unsigned int p_6) {
  int count = 0;
  for (p_6 = 1; p_6 < 3; p_6 = my_add(p_6, 1)) {
    if (count++ > 1)
      abort();

    if (my_shift(p_6, p_6))
      return 0;
  }
  return 0;
}

int main(void) {
  func_4(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn my_add(mut si1: u32, mut si2: u32) -> u32 {
// CHECK-NEXT:     if si1 > ((50 as i32) as u32).wrapping_sub(si2) {
// CHECK-NEXT:         si1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         si1.wrapping_add(si2)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn my_shift(mut left: u32, mut right: u32) -> u32 {
// CHECK-NEXT:     if right > ((100 as i32) as u32) { left } else { left >> right }
// CHECK-NEXT: }
// CHECK-NEXT: fn func_4(mut p_6: u32) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0 as i32;
// CHECK-NEXT:     p_6 = (1 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while p_6 < ((3 as i32) as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:         if [[V_0]] > (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if my_shift(p_6, p_6) != (0 as u32) {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         p_6 = my_add(p_6, (1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func_4((0 as i32) as u32);
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
