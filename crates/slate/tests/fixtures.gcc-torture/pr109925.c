/* PR tree-optimization/109925 */

int a, c, f;

int main() {
  int g[2];
  for (c = 0; c < 2; c++) {
    {
      char h[20], *b = h;
      int  d = 48, e = 0;
      while (d && e < 5)
        b[e++] = d /= 10;
      f = e;
    }
    g[f - 2 + c] = 0;
  }
  for (;;) {
    for (; a <= 4; a++)
      if (g[0])
        break;
    break;
  }
  if (a != 5)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/109925
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut g: [i32; 2] = [0 as i32; 2];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { c }) < (2 as i32) {
// CHECK-NEXT:         let mut h: __SlateAlign16<[i8; 20]> = __SlateAlign16([0 as i8; 20]);
// CHECK-NEXT:         let mut b: *mut i8 = h.0.as_mut_ptr() as *mut i8;
// CHECK-NEXT:         let mut d: i32 = 48 as i32;
// CHECK-NEXT:         let mut e: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while d != (0 as i32) && e < (5 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] / (10 as i32);
// CHECK-NEXT:             d = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             e = [[V_3]];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { b.offset([[V_2]] as isize) } = [[V_1]] as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = e;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 (g.as_mut_ptr() as *mut i32)
// CHECK-NEXT:                     .offset(((unsafe { f }) - (2 as i32) + unsafe { c }) as isize)
// CHECK-NEXT:             } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while (unsafe { a }) <= (4 as i32) {
// CHECK-NEXT:             if (unsafe { *unsafe { (g.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 != (0 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 break '[[SLATE_BREAK_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = [[V_7]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         break '[[SLATE_BREAK_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (5 as i32) {
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
