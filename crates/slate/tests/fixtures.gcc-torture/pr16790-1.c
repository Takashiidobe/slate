/* PR middle-end/16790.  */

extern void abort();

static void test1(unsigned int u1) {
  unsigned int y_final_1;
  signed short y_middle;
  unsigned int y_final_2;

  y_final_1 = (unsigned int)((signed short)(u1 * 2) * 3);
  y_middle  = (signed short)(u1 * 2);
  y_final_2 = (unsigned int)(y_middle * 3);

  if (y_final_1 != y_final_2)
    abort();
}

static void test2(unsigned int u1) {
  unsigned int y_final_1;
  signed short y_middle;
  unsigned int y_final_2;

  y_final_1 = (unsigned int)((signed short)(u1 << 1) * 3);
  y_middle  = (signed short)(u1 << 1);
  y_final_2 = (unsigned int)(y_middle * 3);

  if (y_final_1 != y_final_2)
    abort();
}

int main() {
  test1(0x4000U);
  test2(0x4000U);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/16790.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut u1: u32) {
// CHECK-NEXT:     let mut y_final_1: u32 =
// CHECK-NEXT:         ((((u1.wrapping_mul((2 as i32) as u32) as u16) as i16) as i32) * (3 as i32)) as u32;
// CHECK-NEXT:     let mut y_middle: i16 = (u1.wrapping_mul((2 as i32) as u32) as u16) as i16;
// CHECK-NEXT:     let mut y_final_2: u32 = ((y_middle as i32) * (3 as i32)) as u32;
// CHECK-NEXT:     if y_final_1 != y_final_2 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut u1: u32) {
// CHECK-NEXT:     let mut y_final_1: u32 = (((((u1 << (1 as i32)) as u16) as i16) as i32) * (3 as i32)) as u32;
// CHECK-NEXT:     let mut y_middle: i16 = ((u1 << (1 as i32)) as u16) as i16;
// CHECK-NEXT:     let mut y_final_2: u32 = ((y_middle as i32) * (3 as i32)) as u32;
// CHECK-NEXT:     if y_final_1 != y_final_2 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1(16384 as u32);
// CHECK-NEXT:     test2(16384 as u32);
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
