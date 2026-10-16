/* PR rtl-optimization/97386 */

__attribute__((noipa)) unsigned char foo(unsigned int c) {
  return __builtin_bswap16(
      (unsigned long long)(0xccccLLU << c | 0xccccLLU >> ((-c) & 63)));
}

int main() {
  unsigned char x = foo(0);
  if (__CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && x != 0xcc)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/97386
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i16"]
// CHECK-NEXT:     fn __slate_intrinsic_1a0c079c71348b00([[ARG_0:arg[0-9]+]]: u16) -> u16;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut c: u32) -> u8 {
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         __slate_intrinsic_1a0c079c71348b00(
// CHECK-NEXT:             ((52428 as u64) << c | (52428 as u64) >> (c.wrapping_neg() & ((63 as i32) as u32)))
// CHECK-NEXT:                 as u16,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u8 = foo((0 as i32) as u32);
// CHECK-NEXT:     if (8 as i32) == (8 as i32) && (2 as i32) == (2 as i32) && ((x as u32) as i32) != (204 as i32) {
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
