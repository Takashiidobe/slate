/* PR rtl-optimization/57860 */

extern void abort(void);
int         a, *b = &a, c, d, e, *f = &e, g, *h = &d, k[1] = {1};

int foo(int p) {
  for (;; g++) {
    for (; c; c--)
      ;
    *f = *h = p > ((0x1FFFFFFFFLL ^ a) & *b);
    if (k[g])
      return 0;
  }
}

int main() {
  foo(1);
  if (d != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57860
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: *mut i32 = unsafe { std::ptr::addr_of_mut!(e) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: [i32; 1] = [1 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: i32) -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = ((p as i64)
// CHECK-NEXT:             > ((8589934591 as i64) ^ ((unsafe { a }) as i64)) & ((unsafe { *unsafe { b } }) as i64))
// CHECK-NEXT:             as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { h } = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { f } = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(k) as *mut i32 }.offset((unsafe { g }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe { g };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1 as i32);
// CHECK-NEXT:     if (unsafe { d }) != (1 as i32) {
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
