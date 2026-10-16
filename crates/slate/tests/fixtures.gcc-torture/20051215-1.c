/* PR rtl-optimization/24899 */

extern void abort(void);

__attribute__((noinline)) int foo(int x, int y, int *z) {
  int a, b, c, d;

  a = b = 0;
  for (d = 0; d < y; d++) {
    if (z)
      b = d * *z;
    for (c = 0; c < x; c++)
      a += b;
  }

  return a;
}

int main(void) {
  if (foo(3, 2, 0) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/24899
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: *mut i32) -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while d < y {
// CHECK-NEXT:         if z != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:             b = d * unsafe { *z };
// CHECK-NEXT:         }
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while c < x {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + b;
// CHECK-NEXT:             a = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             c = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         d = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(3 as i32, 2 as i32, std::ptr::null_mut::<i32>()) != (0 as i32) {
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
