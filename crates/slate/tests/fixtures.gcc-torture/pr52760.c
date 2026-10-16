/* PR tree-optimization/52760 */

struct T {
  unsigned short a, b, c, d;
};

__attribute__((noinline, noclone)) void foo(int x, struct T *y) {
  int i;

  for (i = 0; i < x; i++) {
    y[i].a = ((0x00ff & y[i].a >> 8) | (0xff00 & y[i].a << 8));
    y[i].b = ((0x00ff & y[i].b >> 8) | (0xff00 & y[i].b << 8));
    y[i].c = ((0x00ff & y[i].c >> 8) | (0xff00 & y[i].c << 8));
    y[i].d = ((0x00ff & y[i].d >> 8) | (0xff00 & y[i].d << 8));
  }
}

int main() {
  struct T t = {0x0001, 0x0203, 0x0405, 0x0607};
  foo(1, &t);
  if (t.a != 0x0100 || t.b != 0x0302 || t.c != 0x0504 || t.d != 0x0706)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/52760
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     a: u16,
// CHECK-NEXT:     b: u16,
// CHECK-NEXT:     c: u16,
// CHECK-NEXT:     d: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: *mut T) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < x {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { y.offset(i as isize) }).a = (((255 as i32)
// CHECK-NEXT:                 & (((unsafe { (*unsafe { y.offset(i as isize) }).a }) as u32) as i32) >> (8 as i32)
// CHECK-NEXT:                 | (65280 as i32)
// CHECK-NEXT:                     & (((unsafe { (*unsafe { y.offset(i as isize) }).a }) as u32) as i32)
// CHECK-NEXT:                         << (8 as i32)) as i16)
// CHECK-NEXT:                 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { y.offset(i as isize) }).b = (((255 as i32)
// CHECK-NEXT:                 & (((unsafe { (*unsafe { y.offset(i as isize) }).b }) as u32) as i32) >> (8 as i32)
// CHECK-NEXT:                 | (65280 as i32)
// CHECK-NEXT:                     & (((unsafe { (*unsafe { y.offset(i as isize) }).b }) as u32) as i32)
// CHECK-NEXT:                         << (8 as i32)) as i16)
// CHECK-NEXT:                 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { y.offset(i as isize) }).c = (((255 as i32)
// CHECK-NEXT:                 & (((unsafe { (*unsafe { y.offset(i as isize) }).c }) as u32) as i32) >> (8 as i32)
// CHECK-NEXT:                 | (65280 as i32)
// CHECK-NEXT:                     & (((unsafe { (*unsafe { y.offset(i as isize) }).c }) as u32) as i32)
// CHECK-NEXT:                         << (8 as i32)) as i16)
// CHECK-NEXT:                 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { y.offset(i as isize) }).d = (((255 as i32)
// CHECK-NEXT:                 & (((unsafe { (*unsafe { y.offset(i as isize) }).d }) as u32) as i32) >> (8 as i32)
// CHECK-NEXT:                 | (65280 as i32)
// CHECK-NEXT:                     & (((unsafe { (*unsafe { y.offset(i as isize) }).d }) as u32) as i32)
// CHECK-NEXT:                         << (8 as i32)) as i16)
// CHECK-NEXT:                 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: T = T {
// CHECK-NEXT:         a: ((1 as i32) as i16) as u16,
// CHECK-NEXT:         b: ((515 as i32) as i16) as u16,
// CHECK-NEXT:         c: ((1029 as i32) as i16) as u16,
// CHECK-NEXT:         d: ((1543 as i32) as i16) as u16,
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(1 as i32, std::ptr::addr_of_mut!(t));
// CHECK-NEXT:     if ((t.a as u32) as i32) != (256 as i32)
// CHECK-NEXT:         || ((t.b as u32) as i32) != (770 as i32)
// CHECK-NEXT:         || ((t.c as u32) as i32) != (1284 as i32)
// CHECK-NEXT:         || ((t.d as u32) as i32) != (1798 as i32)
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
