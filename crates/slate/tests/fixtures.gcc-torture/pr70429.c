/* PR rtl-optimization/70429 */

__attribute__((noinline, noclone)) int foo(int a) {
  return (int)(0x14ff6e2207db5d1fLL >> a) >> 4;
}

int main() {
  if (sizeof(int) != 4 || sizeof(long long) != 8 || __CHAR_BIT__ != 8)
    return 0;
  if (foo(1) != 0x3edae8 || foo(2) != -132158092)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/70429
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: i32) -> i32 {
// CHECK-NEXT:     (((1513049092259536159 as i64) >> a) as i32) >> (4 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as u64) != (((8 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as i32) != (8 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(1 as i32) != (4119272 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo(2 as i32) != -(132158092 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
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
