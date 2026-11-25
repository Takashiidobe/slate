// { dg-options "/Zc:preprocessor" }
#include <intrin.h>

#pragma intrinsic(_BitScanForward, _BitScanReverse)

static int check32(unsigned long mask) {
  unsigned long index;
  if (!_BitScanForward(&index, mask) || index != 4)
    return 1;
  if (!_BitScanReverse(&index, mask) || index != 31)
    return 2;
  _BitScanForward(&index, mask);
  if (index != 4)
    return 3;
  return _BitScanForward(&index, 0) || _BitScanReverse(&index, 0);
}

#if defined(_M_X64) || defined(_M_ARM64)
#pragma intrinsic(_BitScanForward64, _BitScanReverse64)

static int check64(unsigned __int64 mask) {
  unsigned long index;
  if (!_BitScanForward64(&index, mask) || index != 32)
    return 1;
  if (!_BitScanReverse64(&index, mask) || index != 63)
    return 2;
  return _BitScanForward64(&index, 0) || _BitScanReverse64(&index, 0);
}
#endif

int main(void) {
  int failed = check32(0x80000010UL) != 0;
#if defined(_M_X64) || defined(_M_ARM64)
  failed |= 2 * (check64(0x8000000100000000ULL) != 0);
#endif
  return failed;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check32(mask: u32) -> i32 {
// CHECK-NEXT:     let mut index: u32 = 0;
// CHECK-NEXT:     if ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = mask;
// CHECK-NEXT:         if [[T_1]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_0]].write([[T_1]].trailing_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) == 0 || index != 4 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ({
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = mask;
// CHECK-NEXT:         if [[T_3]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_2]].write(31 - [[T_3]].leading_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) == 0 || index != 31 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = mask;
// CHECK-NEXT:         if [[T_5]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_4]].write([[T_5]].trailing_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     if index != 4 {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     (({
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = 0u32;
// CHECK-NEXT:         if [[T_7]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_6]].write([[T_7]].trailing_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != 0
// CHECK-NEXT:         || ({
// CHECK-NEXT:             let [[T_8:__t[0-9]+]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:             let [[T_9:__t[0-9]+]] = 0u32;
// CHECK-NEXT:             if [[T_9]] == 0 {
// CHECK-NEXT:                 0u8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { [[T_8]].write(31 - [[T_9]].leading_zeros()) };
// CHECK-NEXT:                     1u8
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != 0) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check64(mask: u64) -> i32 {
// CHECK-NEXT:     let mut index: u32 = 0;
// CHECK-NEXT:     if ({
// CHECK-NEXT:         let [[T_0]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_1]] = mask;
// CHECK-NEXT:         if [[T_1]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_0]].write([[T_1]].trailing_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) == 0 || index != 32 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ({
// CHECK-NEXT:         let [[T_2]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_3]] = mask;
// CHECK-NEXT:         if [[T_3]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_2]].write(63 - [[T_3]].leading_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) == 0 || index != 63 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     (({
// CHECK-NEXT:         let [[T_4]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:         let [[T_5]] = 0u64;
// CHECK-NEXT:         if [[T_5]] == 0 {
// CHECK-NEXT:             0u8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { [[T_4]].write([[T_5]].trailing_zeros()) };
// CHECK-NEXT:                 1u8
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != 0
// CHECK-NEXT:         || ({
// CHECK-NEXT:             let [[T_6]]: *mut u32 = core::ptr::addr_of_mut!(index);
// CHECK-NEXT:             let [[T_7]] = 0u64;
// CHECK-NEXT:             if [[T_7]] == 0 {
// CHECK-NEXT:                 0u8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { [[T_6]].write(63 - [[T_7]].leading_zeros()) };
// CHECK-NEXT:                     1u8
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != 0) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut failed: i32 = (check32(2147483664u32) != 0) as i32;
// CHECK-NEXT:     failed |= 2 * ((check64(9223372041149743104u64) != 0) as i32);
// CHECK-NEXT:     failed
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
