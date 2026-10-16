/* PR rtl-optimization/78378 */

unsigned long long __attribute__((noinline, noclone))
foo(unsigned long long x) {
  x <<= 41;
  x  /= 232;
  return 1 + (unsigned short)x;
}

int main() {
  unsigned long long x = foo(1);
  if (x != 0x2c24)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/78378
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64) -> u64 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] << (41 as i32);
// CHECK-NEXT:     x = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]] / (((232 as i32) as i64) as u64);
// CHECK-NEXT:     x = [[V_3]];
// CHECK-NEXT:     (((1 as i32) + (((x as u16) as u32) as i32)) as i64) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = foo(((1 as i32) as i64) as u64);
// CHECK-NEXT:     if x != (((11300 as i32) as i64) as u64) {
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
