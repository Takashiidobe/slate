#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

#ifndef __SIZEOF_INT128__
#define __int128 long long
#endif

/* Some version of bswap optimization would ICE when analyzing a mask constant
   too big for an uint64_t variable (PR210931).  */

__attribute__((noinline, noclone)) uint64_t
uint128_central_bitsi_ior(unsigned __int128 in1, uint64_t in2) {
  __int128 mask = (__int128)0xffff << 56;
  return ((in1 & mask) >> 56) | in2;
}

int main(int argc, char **argv) {
  __int128 in = 1;
#ifdef __SIZEOF_INT128__
  in <<= 64;
#endif
  if (sizeof(uint64_t) * __CHAR_BIT__ != 64)
    return 0;
  if (sizeof(unsigned __int128) * __CHAR_BIT__ != 128)
    return 0;
  if (uint128_central_bitsi_ior(in, 2) != 0x102)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Some version of bswap optimization would ICE when analyzing a mask constant
// CHECK-NEXT: //! too big for an uint64_t variable (PR210931).
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn uint128_central_bitsi_ior(mut in1: u128, mut in2: u64) -> u64 {
// CHECK-NEXT:     let mut mask: i128 = ((65535 as i32) as i128) << (56 as i32);
// CHECK-NEXT:     ((in1 & (mask as u128)) >> (56 as i32) | (in2 as u128)) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut r#in: i128 = (1 as i32) as i128;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i128 = r#in;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i128 = [[V_0]] << (64 as i32);
// CHECK-NEXT:     r#in = [[V_1]];
// CHECK-NEXT:     if (8 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((64 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (16 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((128 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if uint128_central_bitsi_ior(r#in as u128, ((2 as i32) as i64) as u64)
// CHECK-NEXT:         != (((258 as i32) as i64) as u64)
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
