extern void abort();

int f(int x) { return (x >> (sizeof(x) * __CHAR_BIT__ - 1)) ? -1 : 1; }

volatile int one = 1;
int          main(void) {
  /* Test that the function above returns different values for
     different signs.  */
  if (f(one) == f(-one))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut x: i32) -> i32 {
// CHECK-NEXT:     if x >> (4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         -(1 as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut one: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // Test that the function above returns different values for
// CHECK-NEXT:     // different signs.
// CHECK-NEXT:     if f(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:         == f(-unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:     {
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
