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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut u8: u8 = 0;
// LOWERING-NEXT:     let mut i8: i8 = 0;
// LOWERING-NEXT:     let mut u32: u32 = 0;
// LOWERING-NEXT:     let mut i64: i64 = 0;
// LOWERING-NEXT:     let mut old_u8: u8 = 0;
// LOWERING-NEXT:     let mut old_i8: i8 = 0;
// LOWERING-NEXT:     let mut old_u32: u32 = 0;
// LOWERING-NEXT:     let mut old_i64: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 250;
// LOWERING-NEXT:     u8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = -5;
// LOWERING-NEXT:     i8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1000;
// LOWERING-NEXT:     u32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = -10000000000i64;
// LOWERING-NEXT:     i64 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8)).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     old_u8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8)).fetch_sub({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// LOWERING-NEXT:     old_i8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 255;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32)).fetch_xor({{_v[0-9]+}}, std::sync::atomic::Ordering::Release) };
// LOWERING-NEXT:     old_u32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1234567890123i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64)).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     old_i64 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = old_u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = old_i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = old_u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = old_i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut u8: u8 = 0;
// REWRITES-NEXT: let mut i8: i8 = 0;
// REWRITES-NEXT: let mut u32: u32 = 0;
// REWRITES-NEXT: let mut i64: i64 = 0;
// REWRITES-NEXT: let mut old_u8: u8 = 0;
// REWRITES-NEXT: let mut old_i8: i8 = 0;
// REWRITES-NEXT: let mut old_u32: u32 = 0;
// REWRITES-NEXT: let mut old_i64: i64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: u8 = 250;
// REWRITES-NEXT: i8 = -5;
// REWRITES-NEXT: u32 = 1000;
// REWRITES-NEXT: i64 = -10000000000i64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = 3;
// REWRITES-NEXT: old_u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8)).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i8 = 7;
// REWRITES-NEXT: old_i8 = unsafe { std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8)).fetch_sub({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 255;
// REWRITES-NEXT: old_u32 = unsafe { std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32)).fetch_xor({{_v[0-9]+}}, std::sync::atomic::Ordering::Release) };
// REWRITES-NEXT: old_i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64)).swap(1234567890123i64, std::sync::atomic::Ordering::Acquire) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %d %u %lld %u %d %u %lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = old_u8 as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = old_i8 as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = old_u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = old_i64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(u8)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i8 = unsafe { std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(i8)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(u32)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(i64)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
