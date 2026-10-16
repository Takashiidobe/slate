/* PR rtl-optimization/53160 */

extern void abort(void);

int           a, c = 1, d, e, g;
volatile int  b;
volatile char f;
long          h;
short         i;

void foo(void) {
  for (e = 0; e; ++e)
    ;
}

int main() {
  if (g)
    (void)b;
  foo();
  for (d = 0; d >= 0; d--) {
    short j = f;
    int   k = 0;
    i       = j ? j : j << k;
  }
  h = c == 0 ? 0 : i;
  a = h;
  if (a != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/53160
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { g }) != (0 as i32) {
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { d }) >= (0 as i32) {
// CHECK-NEXT:         let mut j: i16 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) }) as i16;
// CHECK-NEXT:         let mut k: i32 = 0 as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = (if j != (0 as i16) {
// CHECK-NEXT:                 j as i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 (j as i32) << k
// CHECK-NEXT:             }) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         h = (if (unsafe { c }) == (0 as i32) {
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (unsafe { i }) as i32
// CHECK-NEXT:         }) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (unsafe { h }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
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
