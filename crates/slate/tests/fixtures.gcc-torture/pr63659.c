/* PR rtl-optimization/63659 */

int           a, b, c, *d = &b, g, h, i;
unsigned char e;
char          f;

int main() {
  while (a) {
    for (a = 0; a; a++)
      for (; c; c++)
        ;
    if (i)
      break;
  }

  char j = c, k = -1, l;
  l = g = j >> h;
  f     = l == 0 ? k : k % l;
  e     = 0 ? 0 : f;
  *d    = e;

  if (b != 255)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/63659
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
// CHECK-NEXT: static mut d: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     c = [[V_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { i }) != (0 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut j: i8 = (unsafe { c }) as i8;
// CHECK-NEXT:     let mut k: i8 = -(1 as i32) as i8;
// CHECK-NEXT:     let mut l: i8 = 0 as i8;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = (j as i32) >> unsafe { h };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     l = [[V_4]] as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = (if (l as i32) == (0 as i32) {
// CHECK-NEXT:             k as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (k as i32) % (l as i32)
// CHECK-NEXT:         }) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = ((if (0 as i32) != (0 as i32) {
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (unsafe { f }) as i32
// CHECK-NEXT:         }) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { d } = ((unsafe { e }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (255 as i32) {
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
