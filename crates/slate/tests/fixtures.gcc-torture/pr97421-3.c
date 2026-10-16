/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched" } */

int   a, b, c;
short d;
void  e(void) {
  unsigned f = 0;
  for (; f <= 2; f++) {
    int g[1];
    int h = (long)g;
    c     = 0;
    for (; c < 10; c++)
      g[0] = a = 0;
    for (; a <= 2; a++)
      b = d;
  }
}
int main(void) {
  e();
  if (a != 3)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/97421
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fmodulo-sched" }
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
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn e() {
// CHECK-NEXT:     let mut f: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while f <= ((2 as i32) as u32) {
// CHECK-NEXT:         let mut g: [i32; 1] = [0 as i32; 1];
// CHECK-NEXT:         ((g.as_mut_ptr() as *mut i32) as i64) as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { c }) < (10 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (g.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { a }) <= (2 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b = (unsafe { d }) as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = f;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         f = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     e();
// CHECK-NEXT:     if (unsafe { a }) != (3 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
