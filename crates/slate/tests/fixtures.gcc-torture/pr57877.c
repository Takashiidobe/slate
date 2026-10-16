/* PR rtl-optimization/57877 */

extern void abort(void);
int         a, b, *c = &b, e, f = 6, g, h;
short       d;

static unsigned char foo(unsigned long long p1, int *p2) {
  for (; g <= 0; g++) {
    short *i = &d;
    int   *j = &e;
    h        = *c;
    *i       = h;
    *j       = (*i == *p2) < p1;
  }
  return 0;
}

int main() {
  foo(f, &a);
  if (e != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57877
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = 6 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p1: u64, mut p2: *mut i32) -> u8 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { g }) <= (0 as i32) {
// CHECK-NEXT:         let mut i: *mut i16 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-NEXT:         let mut j: *mut i32 = unsafe { std::ptr::addr_of_mut!(e) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             h = unsafe { *unsafe { c } };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *i = (unsafe { h }) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *j = (((((((unsafe { *i }) as i32) == unsafe { *p2 }) as i32) as i64) as u64) < p1)
// CHECK-NEXT:                 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { g };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     ((0 as i32) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(((unsafe { f }) as i64) as u64, unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(a)
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { e }) != (1 as i32) {
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
