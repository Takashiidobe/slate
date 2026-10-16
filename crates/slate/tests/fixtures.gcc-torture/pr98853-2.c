/* PR target/98853 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
__attribute__((noipa)) unsigned long long foo(unsigned long long x,
                                              unsigned int       y) {
  return ((unsigned)x & 0xfffe0000U) | (y & 0x1ffff);
}
#endif

int main() {
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
  if (foo(0xdeadbeefcaf2babeULL, 0xdeaffeedU) != 0x00000000caf3feedULL)
    __builtin_abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/98853
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64, mut y: u32) -> u64 {
// CHECK-NEXT:     ((x as u32) & (4294836224 as u32) | y & ((131071 as i32) as u32)) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(16045690984502311614 as u64, 3736076013 as u32) != (3404988141 as u64) {
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
