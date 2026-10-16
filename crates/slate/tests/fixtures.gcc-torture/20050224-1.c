/* Origin: Mikael Pettersson <mikpe@csd.uu.se> and the Linux kernel.  */

extern void   abort(void);
unsigned long a = 0xc0000000, b = 0xd0000000;
unsigned long c = 0xc01bb958, d = 0xc0264000;
unsigned long e = 0xc0288000, f = 0xc02d4378;

void foo(int x, int y, int z) {
  if (x != 245 || y != 36 || z != 444)
    abort();
}

int main(void) {
  unsigned long g;
  int           h = 0, i = 0, j = 0;

  if (sizeof(unsigned long) < 4)
    return 0;

  for (g = a; g < b; g += 0x1000)
    if (g < c)
      h++;
    else if (g >= d && g < e)
      j++;
    else if (g < f)
      i++;
  foo(i, j, h);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: Mikael Pettersson <mikpe@csd.uu.se> and the Linux kernel.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u64 = (3221225472 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u64 = (3489660928 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: u64 = (3223042392 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u64 = (3223732224 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u64 = (3223879680 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: u64 = (3224191864 as u32) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32) {
// CHECK-NEXT:     if x != (245 as i32) || y != (36 as i32) || z != (444 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut g: u64 = 0 as u64;
// CHECK-NEXT:     let mut h: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     if (8 as u64) < (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     g = unsafe { a };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while g < unsafe { b } {
// CHECK-NEXT:         if g < unsafe { c } {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = h;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             h = [[V_1]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if g >= unsafe { d } && g < unsafe { e } {
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:                 j = [[V_3]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if g < unsafe { f } {
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:                     i = [[V_5]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u64 = g;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u64 = [[V_6]].wrapping_add(((4096 as i32) as i64) as u64);
// CHECK-NEXT:         g = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(i, j, h);
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
