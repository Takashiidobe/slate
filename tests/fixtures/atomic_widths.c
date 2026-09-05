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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum memory_order {
// LOWERING-NEXT:     memory_order_relaxed = 0,
// LOWERING-NEXT:     memory_order_consume = 1,
// LOWERING-NEXT:     memory_order_acquire = 2,
// LOWERING-NEXT:     memory_order_release = 3,
// LOWERING-NEXT:     memory_order_acq_rel = 4,
// LOWERING-NEXT:     memory_order_seq_cst = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut u8: u8 = 0;
// LOWERING-NEXT:     let mut i8: i8 = 0;
// LOWERING-NEXT:     let mut u32: u32 = 0;
// LOWERING-NEXT:     let mut i64: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 250;
// LOWERING-NEXT:     u8 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -5;
// LOWERING-NEXT:     i8 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1000;
// LOWERING-NEXT:     u32 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -10000000000i64;
// LOWERING-NEXT:     i64 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// LOWERING-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// LOWERING-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 255;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// LOWERING-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1234567890123i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Acquire)
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum memory_order {
// REWRITES-NEXT:     memory_order_relaxed = 0,
// REWRITES-NEXT:     memory_order_consume = 1,
// REWRITES-NEXT:     memory_order_acquire = 2,
// REWRITES-NEXT:     memory_order_release = 3,
// REWRITES-NEXT:     memory_order_acq_rel = 4,
// REWRITES-NEXT:     memory_order_seq_cst = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut u8: u8 = 0;
// REWRITES-NEXT:     let mut i8: i8 = 0;
// REWRITES-NEXT:     let mut u32: u32 = 0;
// REWRITES-NEXT:     let mut i64: i64 = 0;
// REWRITES-NEXT:     u8 = 250;
// REWRITES-NEXT:     i8 = -5;
// REWRITES-NEXT:     u32 = 1000;
// REWRITES-NEXT:     i64 = -10000000000i64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = 3;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// REWRITES-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// REWRITES-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 255;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// REWRITES-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// REWRITES-NEXT:             .swap(1234567890123i64, std::sync::atomic::Ordering::Acquire)
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%u %d %u %lld %u %d %u %lld\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%u %d %u %lld %u %d %u %lld\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
