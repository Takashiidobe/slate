/* { dg-options "-fno-builtin-abort" } */

int           a, b, m, n, o, p, s, u, i;
char          c, q, y;
short         d;
unsigned char e;
static int    f, h;
static short  g, r, v;
unsigned      t;

extern void abort();

int fn1(int p1) { return a ? p1 : p1 + a; }

unsigned char fn2(unsigned char p1, int p2) { return p2 >= 2 ? p1 : p1 >> p2; }

static short fn3() {
  int w, x = 0;
  for (; p < 31; p++) {
    s = fn1(c | ((1 && c) == c));
    t = fn2(s, x);
    c = (unsigned)c > -(unsigned)((o = (m = d = t) == p) <= 4UL) && n;
    v = -c;
    y = 1;
    for (; y; y++)
      e = v == 1;
    d = 0;
    for (; h != 2;) {
      for (;;) {
        if (!m)
          abort();
        r = 7 - f;
        x = e = i | r;
        q     = u * g;
        w     = b == q;
        if (w)
          break;
      }
      break;
    }
  }
  return x;
}

int main() {
  fn3();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fno-builtin-abort" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut o: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1(mut p1: i32) -> i32 {
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         p1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         p1 + unsafe { a }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2(mut p1: u8, mut p2: i32) -> u8 {
// CHECK-NEXT:     ((if p2 >= (2 as i32) {
// CHECK-NEXT:         (p1 as u32) as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ((p1 as u32) as i32) >> p2
// CHECK-NEXT:     }) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3() -> i16 {
// CHECK-NEXT:     let mut w: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { p }) < (31 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             s = fn1(((unsafe { c }) as i32)
// CHECK-NEXT:                 | (((((1 as i32) != (0 as i32) && (unsafe { c }) != (0 as i8)) as i32)
// CHECK-NEXT:                     == ((unsafe { c }) as i32)) as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             t = fn2(((unsafe { s }) as i8) as u8, x) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = ((unsafe { t }) as u16) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             m = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = ([[V_1]] == unsafe { p }) as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = ((((unsafe { c }) as i32) as u32)
// CHECK-NEXT:                 > (((([[V_2]] as i64) as u64) <= (4 as u64)) as u32).wrapping_neg()
// CHECK-NEXT:                 && (unsafe { n }) != (0 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             v = -((unsafe { c }) as i32) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             y = (1 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { y }) != (0 as i8) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 e = (((unsafe { v }) as i32) == (1 as i32)) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i8 = unsafe { y };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i8 = (([[V_3]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 y = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = (0 as i32) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { h }) != (2 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if !((unsafe { m }) != (0 as i32)) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     r = ((7 as i32) - unsafe { f }) as i16;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: u8 = (((unsafe { i }) | ((unsafe { r }) as i32)) as i8) as u8;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     e = [[V_5]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 x = ([[V_5]] as u32) as i32;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     q = ((unsafe { u }) * ((unsafe { g }) as i32)) as i8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 w = ((unsafe { b }) == ((unsafe { q }) as i32)) as i32;
// CHECK-NEXT:                 if w != (0 as i32) {
// CHECK-NEXT:                     break '[[SLATE_BREAK_3]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             break '[[SLATE_BREAK_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = unsafe { p };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             p = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn3();
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
