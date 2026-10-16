/* { dg-require-effective-target stdint_types } */
#include <stdint.h>

void abort(void);

typedef int32_t __attribute__((vector_size(8))) v2si;
int64_t                                         s64;

static inline int64_t __ev_convert_s64(v2si a) { return (int64_t)a; }

int main() {
  union {
    int64_t ll;
    int32_t i[2];
  } endianness_test;
  endianness_test.ll    = 1;
  int32_t little_endian = endianness_test.i[0];
  s64                   = __ev_convert_s64((v2si){1, 0xffffffff});
  if (s64 != (little_endian ? 0xffffffff00000001LL : 0x1ffffffffLL))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target stdint_types }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     ll: i64,
// CHECK-NEXT:     i: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s64: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __ev_convert_s64(mut a: std::simd::Simd<i32, 2>) -> i64 {
// CHECK-NEXT:     unsafe { std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(a) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut endianness_test: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         endianness_test.ll = (1 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut little_endian: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { endianness_test.i.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s64 = __ev_convert_s64(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                     std::simd::Simd::from_array([1 as i32, (4294967295 as u32) as i32]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         });
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { s64 }) as u64)
// CHECK-NEXT:         != if little_endian != (0 as i32) {
// CHECK-NEXT:             18446744069414584321 as u64
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (8589934591 as i64) as u64
// CHECK-NEXT:         }
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
