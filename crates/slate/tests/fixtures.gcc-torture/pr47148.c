/* PR tree-optimization/47148 */

static inline unsigned bar(unsigned x, unsigned y) {
  if (y >= 32)
    return x;
  else
    return x >> y;
}

static unsigned a = 1, b = 1;

static inline void foo(unsigned char x, unsigned y) {
  if (!y)
    return;
  unsigned c  = (0x7000U / (x - 2)) ^ a;
  unsigned d  = bar(a, a);
  b          &= ((a - d) && (a - 1)) + c;
}

int main(void) {
  foo(1, 1);
  foo(-1, 1);
  if (b && ((unsigned char)-1) == 255)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/47148
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32, mut y: u32) -> u32 {
// CHECK-NEXT:     if y >= ((32 as i32) as u32) {
// CHECK-NEXT:         return x;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return x >> y;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u8, mut y: u32) {
// CHECK-NEXT:     if !(y != (0 as u32)) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: u32 = (28672 as u32) / ((((x as u32) as i32) - (2 as i32)) as u32) ^ unsafe { a };
// CHECK-NEXT:     let mut d: u32 = bar(unsafe { a }, unsafe { a });
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { b };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]]
// CHECK-NEXT:         & (((unsafe { a }.wrapping_sub(d) != (0 as u32)
// CHECK-NEXT:             && unsafe { a }.wrapping_sub((1 as i32) as u32) != (0 as u32)) as i32)
// CHECK-NEXT:             as u32)
// CHECK-NEXT:             .wrapping_add(c);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(((1 as i32) as i8) as u8, (1 as i32) as u32);
// CHECK-NEXT:     foo((-(1 as i32) as i8) as u8, (1 as i32) as u32);
// CHECK-NEXT:     if (unsafe { b }) != (0 as u32) && ((((-(1 as i32) as i8) as u8) as u32) as i32) == (255 as i32)
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
