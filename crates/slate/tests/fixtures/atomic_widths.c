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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum memory_order {
// COMMON-LOWERING-NEXT:     memory_order_relaxed = 0,
// COMMON-LOWERING-NEXT:     memory_order_consume = 1,
// COMMON-LOWERING-NEXT:     memory_order_acquire = 2,
// COMMON-LOWERING-NEXT:     memory_order_release = 3,
// COMMON-LOWERING-NEXT:     memory_order_acq_rel = 4,
// COMMON-LOWERING-NEXT:     memory_order_seq_cst = 5,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut u8: u8 = 0;
// COMMON-LOWERING-NEXT:     let mut i8: i8 = 0;
// COMMON-LOWERING-NEXT:     let mut u32: u32 = 0;
// COMMON-LOWERING-NEXT:     let mut i64: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 250;
// COMMON-LOWERING-NEXT:     u8 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -5;
// COMMON-LOWERING-NEXT:     i8 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1000;
// COMMON-LOWERING-NEXT:     u32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -10000000000i64;
// COMMON-LOWERING-NEXT:     i64 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// COMMON-LOWERING-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// COMMON-LOWERING-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 255;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// COMMON-LOWERING-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1234567890123i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// COMMON-LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Acquire)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum memory_order {
// COMMON-REWRITES-NEXT:     memory_order_relaxed = 0,
// COMMON-REWRITES-NEXT:     memory_order_consume = 1,
// COMMON-REWRITES-NEXT:     memory_order_acquire = 2,
// COMMON-REWRITES-NEXT:     memory_order_release = 3,
// COMMON-REWRITES-NEXT:     memory_order_acq_rel = 4,
// COMMON-REWRITES-NEXT:     memory_order_seq_cst = 5,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut u8: u8 = 0;
// COMMON-REWRITES-NEXT:     let mut i8: i8 = 0;
// COMMON-REWRITES-NEXT:     let mut u32: u32 = 0;
// COMMON-REWRITES-NEXT:     let mut i64: i64 = 0;
// COMMON-REWRITES-NEXT:     u8 = 250;
// COMMON-REWRITES-NEXT:     i8 = -5;
// COMMON-REWRITES-NEXT:     u32 = 1000;
// COMMON-REWRITES-NEXT:     i64 = -10000000000i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = 3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// COMMON-REWRITES-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// COMMON-REWRITES-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 255;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// COMMON-REWRITES-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// COMMON-REWRITES-NEXT:             .swap(1234567890123i64, std::sync::atomic::Ordering::Acquire)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%u %d %u %lld %u %d %u %lld\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%u %d %u %lld %u %d %u %lld\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
