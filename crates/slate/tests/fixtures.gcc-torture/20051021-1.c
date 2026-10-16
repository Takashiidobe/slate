/* Verify that TRUTH_AND_EXPR is not wrongly changed to TRUTH_ANDIF_EXPR.  */

extern void abort(void);

int count = 0;

int foo1(void) {
  count++;
  return 0;
}

int foo2(void) {
  count++;
  return 0;
}

int main(void) {
  if ((foo1() == 1) & (foo2() == 1))
    abort();

  if (count != 2)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Verify that TRUTH_AND_EXPR is not wrongly changed to TRUTH_ANDIF_EXPR.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut count: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo1() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo2() -> i32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((foo1() == (1 as i32)) as i32) & ((foo2() == (1 as i32)) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
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
