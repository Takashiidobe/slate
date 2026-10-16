/* PR c/96549 */

long c = -1L;
long b = 0L;

int main() {
  if (3L > (short)((c ^= (b = 1L)) * 3L))
    return 0;
  __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/96549
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = -(1 as i64);
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i64 = 0 as i64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64 = unsafe { c };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 1 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] ^ (1 as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (3 as i64) > (((([[V_1]] * (3 as i64)) as i16) as i32) as i64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_abort() };
// CHECK-NEXT:     0
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
