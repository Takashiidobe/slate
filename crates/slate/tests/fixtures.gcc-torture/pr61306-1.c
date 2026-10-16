#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#else
typedef int int32_t;
#endif

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned uint32_t;
#endif

#define __fake_const_swab32(x)                                                 \
  ((uint32_t)((((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |               \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >> 8) |                \
              (((int32_t)(x) & (int32_t)0xff000000UL) >> 24)))

/* Previous version of bswap optimization failed to consider sign extension
   and as a result would replace an expression *not* doing a bswap by a
   bswap.  */

__attribute__((noinline, noclone)) uint32_t fake_bswap32(uint32_t in) {
  return __fake_const_swab32(in);
}

int main(void) {
  if (sizeof(int32_t) * __CHAR_BIT__ != 32)
    return 0;
  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  if (fake_bswap32(0x87654321) != 0xffffff87)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Previous version of bswap optimization failed to consider sign extension
// CHECK-NEXT: //! and as a result would replace an expression *not* doing a bswap by a
// CHECK-NEXT: //! bswap.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fake_bswap32(mut r#in: u32) -> u32 {
// CHECK-NEXT:     (r#in & ((255 as u64) as u32)) << (24 as i32)
// CHECK-NEXT:         | (r#in & ((65280 as u64) as u32)) << (8 as i32)
// CHECK-NEXT:         | (r#in & ((16711680 as u64) as u32)) >> (8 as i32)
// CHECK-NEXT:         | ((((r#in as i32) & (((4278190080 as u64) as u32) as i32)) >> (24 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if fake_bswap32(2271560481 as u32) != (4294967175 as u32) {
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
