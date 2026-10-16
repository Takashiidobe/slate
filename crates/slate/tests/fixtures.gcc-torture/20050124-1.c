/* PR rtl-optimization/19579 */

extern void abort(void);

int foo(int i, int j) {
  int k = i + 1;

  if (j) {
    if (k > 0)
      k++;
    else if (k < 0)
      k--;
  }

  return k;
}

int main(void) {
  if (foo(-2, 0) != -1)
    abort();
  if (foo(-1, 0) != 0)
    abort();
  if (foo(0, 0) != 1)
    abort();
  if (foo(1, 0) != 2)
    abort();
  if (foo(-2, 1) != -2)
    abort();
  if (foo(-1, 1) != 0)
    abort();
  if (foo(0, 1) != 2)
    abort();
  if (foo(1, 1) != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/19579
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i32, mut j: i32) -> i32 {
// CHECK-NEXT:     let mut k: i32 = i + (1 as i32);
// CHECK-NEXT:     if j != (0 as i32) {
// CHECK-NEXT:         if k > (0 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             k = [[V_1]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if k < (0 as i32) {
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:                 k = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     k
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(-(2 as i32), 0 as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(-(1 as i32), 0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(0 as i32, 0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(1 as i32, 0 as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(-(2 as i32), 1 as i32) != -(2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(-(1 as i32), 1 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(0 as i32, 1 as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(1 as i32, 1 as i32) != (3 as i32) {
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
