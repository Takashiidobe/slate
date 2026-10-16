/* PR tree-optimization/106523 */

__attribute__((noipa)) unsigned char f7(unsigned char x, unsigned int y) {
  unsigned int t = x;
  return (t << y) | (t >> ((-y) & 7));
}

int main() {
  if (__CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;

  volatile unsigned char x = 152;
  volatile unsigned int  y = 19;
  if (f7(x, y) != 4)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/106523
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f7(mut x: u8, mut y: u32) -> u8 {
// CHECK-NEXT:     let mut t: u32 = x as u32;
// CHECK-NEXT:     (t << y | t >> (y.wrapping_neg() & ((7 as i32) as u32))) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as i32) != (8 as i32) || (4 as i32) != (4 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: u8 = ((152 as i32) as i8) as u8;
// CHECK-NEXT:     let mut y: u32 = (19 as i32) as u32;
// CHECK-NEXT:     if ((f7(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) },
// CHECK-NEXT:     ) as u32) as i32)
// CHECK-NEXT:         != (4 as i32)
// CHECK-NEXT:     {
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
