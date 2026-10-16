/* PR target/98853 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 &&                        \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
__attribute__((__noipa__)) unsigned long long
foo(unsigned x, unsigned long long y, unsigned long long z) {
  __builtin_memcpy(2 + (char *)&x, 2 + (char *)&y, 2);
  return x + z;
}
#endif

int main() {
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 &&                        \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  if (foo(0x44444444U, 0x1111111111111111ULL, 0x2222222222222222ULL) !=
      0x2222222233336666ULL)
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
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32, mut y: u64, mut z: u64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (std::ptr::addr_of_mut!(x) as *mut i8).offset((2 as i32) as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             (unsafe { (std::ptr::addr_of_mut!(y) as *mut i8).offset((2 as i32) as isize) })
// CHECK-NEXT:                 as *const (),
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     (x as u64).wrapping_add(z)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         1145324612 as u32,
// CHECK-NEXT:         1229782938247303441 as u64,
// CHECK-NEXT:         2459565876494606882 as u64,
// CHECK-NEXT:     ) != (2459565876780951142 as u64)
// CHECK-NEXT:     {
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
