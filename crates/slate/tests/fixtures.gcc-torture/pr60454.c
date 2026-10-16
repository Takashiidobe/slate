#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned uint32_t;
#endif

#define __fake_const_swab32(x)                                                 \
  ((uint32_t)((((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |               \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL)) |                     \
              (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

/* Previous version of bswap optimization would detect byte swap when none
   happen. This test aims at catching such wrong detection to avoid
   regressions.  */

__attribute__((noinline, noclone)) uint32_t fake_swap32(uint32_t in) {
  return __fake_const_swab32(in);
}

int main(void) {
  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  if (fake_swap32(0x12345678UL) != 0x78567E12UL)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Previous version of bswap optimization would detect byte swap when none
// CHECK-NEXT: //! happen. This test aims at catching such wrong detection to avoid
// CHECK-NEXT: //! regressions.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fake_swap32(mut r#in: u32) -> u32 {
// CHECK-NEXT:     (r#in & ((255 as u64) as u32)) << (24 as i32)
// CHECK-NEXT:         | (r#in & ((65280 as u64) as u32)) << (8 as i32)
// CHECK-NEXT:         | (r#in & ((255 as u64) as u32)) << (8 as i32)
// CHECK-NEXT:         | r#in & ((65280 as u64) as u32)
// CHECK-NEXT:         | (r#in & ((4278190080 as u64) as u32)) >> (24 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (fake_swap32((305419896 as u64) as u32) as u64) != (2018934290 as u64) {
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
