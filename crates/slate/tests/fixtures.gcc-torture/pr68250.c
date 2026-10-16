/* PR rtl-optimization/68250 */

signed char a, b, h, k, l, m, o;
short       c, d, n;
int         e, f, g, j, q;

void fn1(void) {
  int p = b || a;
  n     = o > 0 || d > 1 >> o ? d : d << o;
  for (; j; j++)
    m = c < 0 || m || c << p;
  l = f + 1;
  for (; f < 1; f = 1)
    k = h + 1;
}

__attribute__((noinline, noclone)) void fn2(int k) {
  if (k != 1)
    __builtin_abort();
}

int main() {
  signed char i;
  for (; e < 1; e++) {
    fn1();
    if (k)
      i = k;
    if (i > q)
      g = 0;
  }
  fn2(k);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/68250
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut o: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() {
// CHECK-NEXT:     let mut p: i32 = ((unsafe { b }) != (0 as i8) || (unsafe { a }) != (0 as i8)) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         n = (if ((unsafe { o }) as i32) > (0 as i32)
// CHECK-NEXT:             || ((unsafe { d }) as i32) > (1 as i32) >> ((unsafe { o }) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             (unsafe { d }) as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             ((unsafe { d }) as i32) << ((unsafe { o }) as i32)
// CHECK-NEXT:         }) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { j }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             m = (((unsafe { c }) as i32) < (0 as i32)
// CHECK-NEXT:                 || (unsafe { m }) != (0 as i8)
// CHECK-NEXT:                 || ((unsafe { c }) as i32) << p != (0 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { j };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         l = ((unsafe { f }) + (1 as i32)) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { f }) < (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = (((unsafe { h }) as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2(mut [[K_0:k_[0-9]+]]: i32) {
// CHECK-NEXT:     if [[K_0]] != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i8 = 0 as i8;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { e }) < (1 as i32) {
// CHECK-NEXT:         fn1();
// CHECK-NEXT:         if (unsafe { k }) != (0 as i8) {
// CHECK-NEXT:             i = unsafe { k };
// CHECK-NEXT:         }
// CHECK-NEXT:         if (i as i32) > unsafe { q } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 g = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2((unsafe { k }) as i32);
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
