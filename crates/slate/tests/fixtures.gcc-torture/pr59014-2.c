/* PR tree-optimization/59014 */

__attribute__((noinline, noclone)) long long int foo(long long int x,
                                                     long long int y) {
  if (((int)x | (int)y) != 0)
    return 6;
  return x + y;
}

int main() {
  if (sizeof(long long) == sizeof(int))
    return 0;
  int           shift_half = sizeof(int) * __CHAR_BIT__ / 2;
  long long int x          = (3LL << shift_half) << shift_half;
  long long int y          = (5LL << shift_half) << shift_half;
  long long int z          = foo(x, y);
  if (z != ((8LL << shift_half) << shift_half))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/59014
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i64, mut y: i64) -> i64 {
// CHECK-NEXT:     if (x as i32) | (y as i32) != (0 as i32) {
// CHECK-NEXT:         return (6 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     x + y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) == (4 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut shift_half: i32 = (((4 as u64).wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         / (((2 as i32) as i64) as u64)) as u32) as i32;
// CHECK-NEXT:     let mut x: i64 = (3 as i64) << shift_half << shift_half;
// CHECK-NEXT:     let mut y: i64 = (5 as i64) << shift_half << shift_half;
// CHECK-NEXT:     let mut z: i64 = foo(x, y);
// CHECK-NEXT:     if z != (8 as i64) << shift_half << shift_half {
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
