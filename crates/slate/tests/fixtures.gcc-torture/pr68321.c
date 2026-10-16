/* PR rtl-optimization/68321 */

int  e = 1, u = 5, t2, t5, i, k;
int  a[1], b, m;
char n, t;

int fn1(int p1) {
  int g[1];
  for (;;) {
    if (p1 / 3)
      for (; t5;)
        u || n;
    t2 = p1 & 4;
    if (b + 1)
      return 0;
    u = g[0];
  }
}

int main() {
  for (; e >= 0; e--) {
    char c;
    if (!m)
      c = t;
    fn1(c);
  }

  if (a[t2] != 0)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/68321
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: i32 = 5 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut t2: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut t5: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [i32; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1(mut p1: i32) -> i32 {
// CHECK-NEXT:     let mut g: [i32; 1] = [0 as i32; 1];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if p1 / (3 as i32) != (0 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { t5 }) != (0 as i32) {
// CHECK-NEXT:                 (unsafe { u }) != (0 as i32) || (unsafe { n }) != (0 as i8);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             t2 = p1 & (4 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { b }) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             u = unsafe { *unsafe { (g.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { e }) >= (0 as i32) {
// CHECK-NEXT:         let mut c: i8 = 0 as i8;
// CHECK-NEXT:         if !((unsafe { m }) != (0 as i32)) {
// CHECK-NEXT:             c = unsafe { t };
// CHECK-NEXT:         }
// CHECK-NEXT:         fn1(c as i32);
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { t2 }) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i32)
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
