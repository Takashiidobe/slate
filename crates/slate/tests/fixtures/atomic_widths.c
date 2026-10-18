#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_uchar u8  = 250;
  atomic_schar i8  = -5;
  atomic_uint  u32 = 1000u;
  atomic_llong i64 = -10000000000LL;

  unsigned char old_u8 =
      atomic_fetch_add_explicit(&u8, 3, memory_order_relaxed);
  signed char  old_i8 = atomic_fetch_sub_explicit(&i8, 7, memory_order_acq_rel);
  unsigned int old_u32 =
      atomic_fetch_xor_explicit(&u32, 0x00FFu, memory_order_release);
  long long old_i64 =
      atomic_exchange_explicit(&i64, 1234567890123LL, memory_order_acquire);

  printf("%u %d %u %lld %u %d %u %lld\n", old_u8, old_i8, old_u32, old_i64,
         (unsigned char)u8, (signed char)i8, (unsigned int)u32, (long long)i64);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u8: u8 = ((250 as i32) as i8) as u8;
// CHECK-NEXT:     let mut i8: i8 = -5;
// CHECK-NEXT:     let mut u32: u32 = 1000;
// CHECK-NEXT:     let mut i64: i64 = -10000000000;
// CHECK-NEXT:     let mut old_u8: u8 = 0;
// CHECK-NEXT:     old_u8 = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = 3 as u8;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u8) as *mut u8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_i8: i8 = 0;
// CHECK-NEXT:     old_i8 = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = 7 as i8;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicI8::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(i8) as *mut i8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_2]], std::sync::atomic::Ordering::AcqRel);
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_u32: u32 = 0;
// CHECK-NEXT:     old_u32 = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = 255 as u32;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u32) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_4]], std::sync::atomic::Ordering::Release);
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_i64: i64 = 0;
// CHECK-NEXT:     old_i64 = unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = 1234567890123 as i64;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(i64) as *mut i64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_6]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u %lld %u %d %u %lld\n".as_ptr(),
// CHECK-NEXT:             (old_u8 as u32) as i32,
// CHECK-NEXT:             old_i8 as i32,
// CHECK-NEXT:             old_u32,
// CHECK-NEXT:             old_i64,
// CHECK-NEXT:             (std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of!(u8) as *mut u8)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst) as u32) as i32,
// CHECK-NEXT:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of!(i8) as *mut i8)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst) as i32,
// CHECK-NEXT:             std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of!(u32) as *mut u32)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of!(i64) as *mut i64)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
