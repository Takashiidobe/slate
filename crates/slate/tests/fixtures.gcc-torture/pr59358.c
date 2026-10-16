/* PR tree-optimization/59358 */

__attribute__((noinline, noclone)) int foo(int *x, int y) {
  int z = *x;
  if (y > z && y <= 16)
    while (y > z)
      z *= 2;
  return z;
}

int main() {
  int i;
  for (i = 1; i < 17; i++) {
    int j = foo(&i, 16);
    int k;
    if (i >= 8 && i <= 15)
      k = 16 + (i - 8) * 2;
    else if (i >= 4 && i <= 7)
      k = 16 + (i - 4) * 4;
    else if (i == 3)
      k = 24;
    else
      k = 16;
    if (j != k)
      __builtin_abort();
    j = foo(&i, 7);
    if (i >= 7)
      k = i;
    else if (i >= 4)
      k = 8 + (i - 4) * 2;
    else if (i == 3)
      k = 12;
    else
      k = 8;
    if (j != k)
      __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/59358
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &i32, mut y: i32) -> i32 {
// CHECK-NEXT:     let mut z: i32 = unsafe { *(x as *const i32) };
// CHECK-NEXT:     if y > z && y <= (16 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while y > z {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = z;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] * (2 as i32);
// CHECK-NEXT:             z = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     z
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (17 as i32) {
// CHECK-NEXT:         let mut j: i32 = foo(unsafe { &(*std::ptr::addr_of_mut!(i)) }, 16 as i32);
// CHECK-NEXT:         let mut k: i32 = 0 as i32;
// CHECK-NEXT:         if i >= (8 as i32) && i <= (15 as i32) {
// CHECK-NEXT:             k = (16 as i32) + (i - (8 as i32)) * (2 as i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i >= (4 as i32) && i <= (7 as i32) {
// CHECK-NEXT:                 k = (16 as i32) + (i - (4 as i32)) * (4 as i32);
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if i == (3 as i32) {
// CHECK-NEXT:                     k = 24 as i32;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     k = 16 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if j != k {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         j = foo(unsafe { &(*std::ptr::addr_of_mut!(i)) }, 7 as i32);
// CHECK-NEXT:         if i >= (7 as i32) {
// CHECK-NEXT:             k = i;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i >= (4 as i32) {
// CHECK-NEXT:                 k = (8 as i32) + (i - (4 as i32)) * (2 as i32);
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if i == (3 as i32) {
// CHECK-NEXT:                     k = 12 as i32;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     k = 8 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if j != k {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
