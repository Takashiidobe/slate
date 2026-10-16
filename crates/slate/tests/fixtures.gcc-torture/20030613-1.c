/* PR optimization/10955 */
/* Originator: <heinrich.brand@fujitsu-siemens.com> */

/* This used to fail on SPARC32 at -O3 because the loop unroller
   wrongly thought it could eliminate a pseudo in a loop, while
   the pseudo was used outside the loop.  */

extern void abort(void);

#define COMPLEX struct CS

COMPLEX {
  long x;
  long y;
};

static COMPLEX CCID(COMPLEX x) {
  COMPLEX a;

  a.x = x.x;
  a.y = x.y;

  return a;
}

static COMPLEX CPOW(COMPLEX x, int y) {
  COMPLEX a;
  a = x;

  while (--y > 0)
    a = CCID(a);

  return a;
}

static int c5p(COMPLEX x) {
  COMPLEX a, b;
  a = CPOW(x, 2);
  b = CCID(CPOW(a, 2));

  return (b.x == b.y);
}

int main(void) {
  COMPLEX x;

  x.x = -7;
  x.y = -7;

  if (!c5p(x))
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/10955
// CHECK-NEXT: //!
// CHECK-NEXT: //! Originator: <heinrich.brand@fujitsu-siemens.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // This used to fail on SPARC32 at -O3 because the loop unroller
// CHECK-NEXT: // wrongly thought it could eliminate a pseudo in a loop, while
// CHECK-NEXT: // the pseudo was used outside the loop.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct CS {
// CHECK-NEXT:     x: i64,
// CHECK-NEXT:     y: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn CCID(mut x: CS) -> CS {
// CHECK-NEXT:     let mut a: CS = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a.x = x.x;
// CHECK-NEXT:     a.y = x.y;
// CHECK-NEXT:     return a;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn CPOW(mut x: CS, mut y: i32) -> CS {
// CHECK-NEXT:     let mut a: CS = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a = x;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = y;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         y = [[V_1]];
// CHECK-NEXT:         if !([[V_1]] > (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         a = CCID(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     return a;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c5p(mut x: CS) -> i32 {
// CHECK-NEXT:     let mut a: CS = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: CS = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a = CPOW(x, 2 as i32);
// CHECK-NEXT:     b = CCID(CPOW(a, 2 as i32));
// CHECK-NEXT:     (b.x == b.y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: CS = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     x.x = -(7 as i32) as i64;
// CHECK-NEXT:     x.y = -(7 as i32) as i64;
// CHECK-NEXT:     if !(c5p(x) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
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
