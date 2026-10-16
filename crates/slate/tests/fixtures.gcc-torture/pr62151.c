/* PR rtl-optimization/62151 */

int   a, c, d, e, f, g, h, i;
short b;

int fn1() {
  b = 0;
  for (;;) {
    int j[2];
    j[f] = 0;
    if (h)
      d = 0;
    else {
      for (; f; f++)
        ;
      for (a = 0; a < 1; a++)
        for (;;) {
          i = b & ((b ^ 1) & 83647) ? b : b - 1;
          g = 1 ? i : 0;
          e = j[0];
          if (c)
            break;
          return 0;
        }
    }
  }
}

int main() {
  fn1();
  if (g != -1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/62151
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (0 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut j: [i32; 2] = [0 as i32; 2];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (j.as_mut_ptr() as *mut i32).offset((unsafe { f }) as isize) } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { h }) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { f }) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = unsafe { f };
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     f = [[V_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { a }) < (1 as i32) {
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         i = if ((unsafe { b }) as i32)
// CHECK-NEXT:                             & ((((unsafe { b }) as i32) ^ (1 as i32)) & (83647 as i32))
// CHECK-NEXT:                             != (0 as i32)
// CHECK-NEXT:                         {
// CHECK-NEXT:                             (unsafe { b }) as i32
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((unsafe { b }) as i32) - (1 as i32)
// CHECK-NEXT:                         };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g = if (1 as i32) != (0 as i32) {
// CHECK-NEXT:                             unsafe { i }
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             0 as i32
// CHECK-NEXT:                         };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         e = unsafe {
// CHECK-NEXT:                             *unsafe { (j.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:                         };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:                         break '[[SLATE_BREAK_3]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     a = [[V_3]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe { g }) != -(1 as i32) {
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
