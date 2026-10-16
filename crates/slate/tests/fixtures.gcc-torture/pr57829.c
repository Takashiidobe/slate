/* PR rtl-optimization/57829 */

__attribute__((noinline, noclone)) int f1(int k) {
  return 2 | ((k - 1) >> ((int)sizeof(int) * __CHAR_BIT__ - 1));
}

__attribute__((noinline, noclone)) long int f2(long int k) {
  return 2L | ((k - 1L) >> ((int)sizeof(long int) * __CHAR_BIT__ - 1));
}

__attribute__((noinline, noclone)) int f3(int k) {
  k &= 63;
  return 4 | ((k + 2) >> 5);
}

int main() {
  if (f1(1) != 2 || f2(1L) != 2L || f3(63) != 6 || f3(1) != 4)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57829
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut k: i32) -> i32 {
// CHECK-NEXT:     (2 as i32) | k - (1 as i32) >> (((4 as u64) as u32) as i32) * (8 as i32) - (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut k: i64) -> i64 {
// CHECK-NEXT:     (2 as i64) | k - (1 as i64) >> (((8 as u64) as u32) as i32) * (8 as i32) - (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut k: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] & (63 as i32);
// CHECK-NEXT:     k = [[V_1]];
// CHECK-NEXT:     (4 as i32) | k + (2 as i32) >> (5 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f1(1 as i32) != (2 as i32) {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = f2(1 as i64) != (2 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = f3(63 as i32) != (6 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = f3(1 as i32) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]] {
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
