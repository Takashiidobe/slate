/* PR rtl-optimization/60116 */
/* Reported by Zhendong Su <su@cs.ucdavis.edu> */

extern void abort(void);

int  a, b, c, d = 1, e, f = 1, h, i, k;
char g, j;

void fn1(void) {
  int l;
  e = 0;
  c = 0;
  for (;;) {
    k = a && b;
    j = k * 54;
    g = j * 147;
    l = ~g + (long long)e && 1;
    if (d)
      c = l;
    else
      h = i = l * 9UL;
    if (f)
      return;
  }
}

int main(void) {
  fn1();
  if (c != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/60116
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Zhendong Su <su@cs.ucdavis.edu>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() {
// CHECK-NEXT:     let mut l: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = ((unsafe { a }) != (0 as i32) && (unsafe { b }) != (0 as i32)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = ((unsafe { k }) * (54 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = (((unsafe { j }) as i32) * (147 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         l = ((!((unsafe { g }) as i32) as i64) + ((unsafe { e }) as i64) != (0 as i64)
// CHECK-NEXT:             && (1 as i32) != (0 as i32)) as i32;
// CHECK-NEXT:         if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = l;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = (((l as i64) as u64).wrapping_mul(9 as u64) as u32) as i32;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 i = [[V_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 h = [[V_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { f }) != (0 as i32) {
// CHECK-NEXT:             return;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe { c }) != (1 as i32) {
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
