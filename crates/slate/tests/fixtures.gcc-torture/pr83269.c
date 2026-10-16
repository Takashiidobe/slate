/* PR tree-optimization/83269 */

int main() {
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ > 4 && __CHAR_BIT__ == 8
  volatile unsigned char a = 1;
  long long              b = 0x80000000L;
  int                    c = -((int)(-b) - (-0x7fffffff * a));
  if (c != 1)
    __builtin_abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/83269
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u8 = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     let mut b: i64 = 2147483648 as i64;
// CHECK-NEXT:     let mut c: i32 = -((-b as i32)
// CHECK-NEXT:         - -(2147483647 as i32)
// CHECK-NEXT:             * (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as u32) as i32));
// CHECK-NEXT:     if c != (1 as i32) {
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
