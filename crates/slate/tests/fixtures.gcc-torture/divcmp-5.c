/* PR middle-end/26561 */

extern void abort(void);

int always_one_1(int a) {
  if (a / 100 >= -999999999)
    return 1;
  else
    return 0;
}

int always_one_2(int a) {
  if (a / 100 < -999999999)
    return 0;
  else
    return 1;
}

int main(void) {
  if (always_one_1(0) != 1)
    abort();

  if (always_one_2(0) != 1)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/26561
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn always_one_1(mut a: i32) -> i32 {
// CHECK-NEXT:     if a / (100 as i32) >= -(999999999 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn always_one_2(mut a: i32) -> i32 {
// CHECK-NEXT:     if a / (100 as i32) < -(999999999 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if always_one_1(0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if always_one_2(0 as i32) != (1 as i32) {
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
