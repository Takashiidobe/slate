/* PR rtl-optimization/78477 */

unsigned       a;
unsigned short b;

unsigned foo(unsigned x) {
  b   = x;
  a >>= (b & 1);
  b   = 1 | (b << 5);
  b >>= 15;
  x   = (unsigned char)b > ((2 - (unsigned char)b) & 1);
  b   = 0;
  return x;
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(short) != 2 || sizeof(int) < 4)
    return 0;
  unsigned x = foo(12345);
  if (x != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/78477
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = x as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { a };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]] >> ((((unsafe { b }) as u32) as i32) & (1 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (((1 as i32) | (((unsafe { b }) as u32) as i32) << (5 as i32)) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u16 = unsafe { b };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u16 = (((([[V_2]] as u32) as i32) >> (15 as i32)) as i16) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x = (((((unsafe { b }) as u8) as u32) as i32)
// CHECK-NEXT:         > (2 as i32) - ((((unsafe { b }) as u8) as u32) as i32) & (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as i32) != (8 as i32)
// CHECK-NEXT:         || (2 as u64) != (((2 as i32) as i64) as u64)
// CHECK-NEXT:         || (4 as u64) < (((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: u32 = foo((12345 as i32) as u32);
// CHECK-NEXT:     if x != ((0 as i32) as u32) {
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
