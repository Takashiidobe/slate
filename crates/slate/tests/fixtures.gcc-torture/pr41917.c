/* PR rtl-optimization/41917 */

extern void  abort(void);
unsigned int a = 1;

int main(void) {
  unsigned int b, c, d;

  if (sizeof(int) != 4 || (int)0xc7d24b5e > 0)
    return 0;

  c = 0xc7d24b5e;
  d = a | -2;
  b = (d == 0) ? c : (c % d);
  if (b != c)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/41917
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: u32 = 0 as u32;
// CHECK-NEXT:     let mut c: u32 = 3352447838 as u32;
// CHECK-NEXT:     let mut d: u32 = 0 as u32;
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64) || ((3352447838 as u32) as i32) > (0 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     d = (unsafe { a }) | (-(2 as i32) as u32);
// CHECK-NEXT:     b = if d == ((0 as i32) as u32) { c } else { c % d };
// CHECK-NEXT:     if b != c {
// CHECK-NEXT:         unsafe { abort() };
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
