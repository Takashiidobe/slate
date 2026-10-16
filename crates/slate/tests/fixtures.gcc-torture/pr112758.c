/* PR rtl-optimization/112758 */

int a = -__INT_MAX__ - 1;

int main() {
  if (-__INT_MAX__ - 1U == 0x80000000ULL) {
    unsigned long long b = 0xffff00ffffffffffULL;
    if ((b & a) != 0xffff00ff80000000ULL)
      __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/112758
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = -(2147483647 as i32) - (1 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((-(2147483647 as i32) as u32).wrapping_sub(1 as u32) as u64) == (2147483648 as u64) {
// CHECK-NEXT:         let mut b: u64 = 18446463698244468735 as u64;
// CHECK-NEXT:         if b & (((unsafe { a }) as i64) as u64) != (18446463696096985088 as u64) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
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
