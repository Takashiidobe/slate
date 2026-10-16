/* PR rtl-optimization/57861 */

extern void  abort(void);
short        a = 1, f;
int          b, c, d, *g = &b, h, i, j;
unsigned int e;

static int foo(char p) {
  int k;
  for (c = 0; c < 2; c++) {
    i = (j = 0) || p;
    k = i * p;
    if (e < k) {
      short *l = &f;
      a        = d && h;
      *l       = 0;
    }
  }
  return 0;
}

int main() {
  *g = foo(a);
  if (a != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57861
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i16 = (1 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: i8) -> i32 {
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { c }) < (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = ((0 as i32) != (0 as i32) || p != (0 as i8)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         k = (unsafe { i }) * (p as i32);
// CHECK-NEXT:         if (unsafe { e }) < (k as u32) {
// CHECK-NEXT:             let mut l: *mut i16 = unsafe { std::ptr::addr_of_mut!(f) };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = ((unsafe { d }) != (0 as i32) && (unsafe { h }) != (0 as i32)) as i16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *l = (0 as i32) as i16;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { g } = foo((unsafe { a }) as i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { a }) as i32) != (0 as i32) {
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
