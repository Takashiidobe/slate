/* PR tree-optimization/98727 */

__attribute__((noipa)) long int foo(long int x, long int y) {
  long int z = (unsigned long)x * y;
  if (x != z / y)
    return -1;
  return z;
}

int main() {
  if (foo(4, 24) != 96 || foo(124, 126) != 124L * 126 ||
      foo(__LONG_MAX__ / 16, 17) != -1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/98727
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i64, mut y: i64) -> i64 {
// CHECK-NEXT:     let mut z: i64 = (x as u64).wrapping_mul(y as u64) as i64;
// CHECK-NEXT:     if x != z / y {
// CHECK-NEXT:         return -(1 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     z
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo((4 as i32) as i64, (24 as i32) as i64) != ((96 as i32) as i64) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] =
// CHECK-NEXT:             foo((124 as i32) as i64, (126 as i32) as i64) != (124 as i64) * ((126 as i32) as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo(
// CHECK-NEXT:             (9223372036854775807 as i64) / ((16 as i32) as i64),
// CHECK-NEXT:             (17 as i32) as i64,
// CHECK-NEXT:         ) != (-(1 as i32) as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
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
