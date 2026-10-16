/* PR rtl-optimization/89634 */

static unsigned long *foo(unsigned long *x) { return x + (1 + *x); }

__attribute__((noipa)) unsigned long bar(unsigned long *x) {
  unsigned long c, d = 1, e, *f, g, h = 0, i;
  for (e = *x - 1; e > 0; e--) {
    f = foo(x + 1);
    for (i = 1; i < e; i++)
      f = foo(f);
    c = *f;
    if (c == 2)
      d *= 2;
    else {
      i = (c - 1) / 2 - 1;
      g = (2 * i + 1) * (d + 1) + (2 * d + 1);
      if (g > h)
        h = g;
      d *= c;
    }
  }
  return h;
}

int main() {
  unsigned long a[18] = {4,    2,   -200, 200, 2,   -400,
                         400,  3,   -600, 0,   600, 5,
                         -100, -66, 0,    66,  100, __LONG_MAX__ / 8 + 1};
  if (bar(a) != 17)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/89634
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut u64) -> *mut u64 {
// CHECK-NEXT:     unsafe { x.offset((((1 as i32) as i64) as u64).wrapping_add(unsafe { *x }) as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut u64) -> u64 {
// CHECK-NEXT:     let mut c: u64 = 0 as u64;
// CHECK-NEXT:     let mut d: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     let mut e: u64 = unsafe { *x }.wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:     let mut f: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut g: u64 = 0 as u64;
// CHECK-NEXT:     let mut h: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     let mut i: u64 = 0 as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while e > (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         f = foo(unsafe { x.offset((1 as i32) as isize) });
// CHECK-NEXT:         i = ((1 as i32) as i64) as u64;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < e {
// CHECK-NEXT:             f = foo(f);
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         c = unsafe { *f };
// CHECK-NEXT:         if c == (((2 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: u64 = d;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_mul(((2 as i32) as i64) as u64);
// CHECK-NEXT:             d = [[V_3]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             i = (c.wrapping_sub(((1 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:             g = (((2 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_mul(i)
// CHECK-NEXT:                 .wrapping_add(((1 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_mul(d.wrapping_add(((1 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_add(
// CHECK-NEXT:                     (((2 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_mul(d)
// CHECK-NEXT:                         .wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:                 );
// CHECK-NEXT:             if g > h {
// CHECK-NEXT:                 h = g;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: u64 = d;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]].wrapping_mul(c);
// CHECK-NEXT:             d = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u64 = e;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u64 = [[V_6]].wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:         e = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     h
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[u64; 18]> = __SlateAlign16([
// CHECK-NEXT:         ((4 as i32) as i64) as u64,
// CHECK-NEXT:         ((2 as i32) as i64) as u64,
// CHECK-NEXT:         (-(200 as i32) as i64) as u64,
// CHECK-NEXT:         ((200 as i32) as i64) as u64,
// CHECK-NEXT:         ((2 as i32) as i64) as u64,
// CHECK-NEXT:         (-(400 as i32) as i64) as u64,
// CHECK-NEXT:         ((400 as i32) as i64) as u64,
// CHECK-NEXT:         ((3 as i32) as i64) as u64,
// CHECK-NEXT:         (-(600 as i32) as i64) as u64,
// CHECK-NEXT:         ((0 as i32) as i64) as u64,
// CHECK-NEXT:         ((600 as i32) as i64) as u64,
// CHECK-NEXT:         ((5 as i32) as i64) as u64,
// CHECK-NEXT:         (-(100 as i32) as i64) as u64,
// CHECK-NEXT:         (-(66 as i32) as i64) as u64,
// CHECK-NEXT:         ((0 as i32) as i64) as u64,
// CHECK-NEXT:         ((66 as i32) as i64) as u64,
// CHECK-NEXT:         ((100 as i32) as i64) as u64,
// CHECK-NEXT:         ((9223372036854775807 as i64) / ((8 as i32) as i64) + ((1 as i32) as i64)) as u64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     if bar(a.0.as_mut_ptr() as *mut u64) != (((17 as i32) as i64) as u64) {
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
