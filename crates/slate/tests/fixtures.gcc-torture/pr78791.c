/* PR target/78791 */

__attribute__((used, noinline, noclone)) unsigned long long
foo(unsigned long long x, unsigned long long y, unsigned long long z) {
  unsigned long long a  = x / y;
  unsigned long long b  = x % y;
  a                    |= z;
  b                    ^= z;
  return a + b;
}

int main() {
  if (foo(64, 7, 0) != 10 || foo(28, 3, 2) != 14)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/78791
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64, mut y: u64, mut z: u64) -> u64 {
// CHECK-NEXT:     let mut a: u64 = x / y;
// CHECK-NEXT:     let mut b: u64 = x % y;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] | z;
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = b;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]] ^ z;
// CHECK-NEXT:     b = [[V_3]];
// CHECK-NEXT:     a.wrapping_add(b)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         ((64 as i32) as i64) as u64,
// CHECK-NEXT:         ((7 as i32) as i64) as u64,
// CHECK-NEXT:         ((0 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (((10 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = foo(
// CHECK-NEXT:             ((28 as i32) as i64) as u64,
// CHECK-NEXT:             ((3 as i32) as i64) as u64,
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:         ) != (((14 as i32) as i64) as u64);
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
