#ifdef __SIZEOF_INT128__
static unsigned __int128 swap128(unsigned __int128 value) {
  return __builtin_bswap128(value);
}
#endif

int main(void) {
#ifdef __SIZEOF_INT128__
  unsigned __int128 value =
      ((unsigned __int128)0x0123456789abcdefULL << 64) | 0x1020304050607080ULL;
  unsigned __int128 swapped = swap128(value);
  return (unsigned long long)(swapped >> 64) != 0x8070605040302010ULL ||
         (unsigned long long)swapped != 0xefcdab8967452301ULL;
#else
  return 0;
#endif
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i128"]
// CHECK-NEXT:     fn __slate_intrinsic_60d63993ffb96535([[ARG_0:arg[0-9]+]]: u128) -> u128;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn swap128(value: u128) -> u128 {
// CHECK-NEXT:     unsafe { __slate_intrinsic_60d63993ffb96535(value) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let value: u128 = (81985529216486895 as u128) << 64i32 | 1161981756646125696;
// CHECK-NEXT:     let swapped: u128 = swap128(value);
// CHECK-NEXT:     (((swapped >> 64i32) as u64) != 9255003132036915216
// CHECK-NEXT:         || (swapped as u64) != 17279655951921914625) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
