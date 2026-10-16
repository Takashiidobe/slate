
int   a, b, d = 1, e, f, o, u, w = 1, z;
short c, q, t;

int main() {
  char g;
  for (; d; d--) {
    while (o)
      for (; e;) {
        c     = b;
        int h = o = z;
        for (; u;)
          for (; a;)
            ;
      }
    if (t < 1)
      g = w;
    f = g;
    g && (q = 1);
  }

  if (q != 1)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = 1 as i32;
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut o: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut u: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut w: i32 = 1 as i32;
// CHECK-NEXT: static mut z: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut q: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut t: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut g: i8 = 0 as i8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { o }) != (0 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     c = (unsafe { b }) as i16;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut h: i32 = 0 as i32;
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = unsafe { z };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     o = [[V_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 h = [[V_0]];
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while (unsafe { u }) != (0 as i32) {
// CHECK-NEXT:                     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:                         {}
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { t }) as i32) < (1 as i32) {
// CHECK-NEXT:             g = (unsafe { w }) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = g as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:         if g != (0 as i8) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 q = (1 as i32) as i16;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[V_1]] = ((1 as i32) as i16) != (0 as i16);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_1]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { q }) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
