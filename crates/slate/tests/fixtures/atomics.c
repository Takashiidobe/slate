#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store(&a, 100);
  int loaded = atomic_load(&a);

  int fa   = atomic_fetch_add(&a, 5);    // returns 100, a = 105
  int fs   = atomic_fetch_sub(&a, 10);   // returns 105, a = 95
  int fand = atomic_fetch_and(&a, 0x3C); // 95 & 60 = 28
  int forr = atomic_fetch_or(&a, 0x01);  // 28 | 1 = 29
  int fxor = atomic_fetch_xor(&a, 0x0F); // 29 ^ 15 = 18

  int xchg_old = atomic_exchange(&a, 7); // returns 18, a = 7

  int expected = 7;
  int ok = atomic_compare_exchange_strong(&a, &expected, 42); // success, a = 42
  int expected2 = 999;
  int bad =
      atomic_compare_exchange_strong(&a, &expected2, 0); // fail, a stays 42

  atomic_thread_fence(memory_order_seq_cst);

  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", loaded, fa, fs, fand, forr,
         fxor, xchg_old, ok, expected, bad, expected2, (int)a);
  return 0;
}

// REWRITES-NOT: _atomictmp
// REWRITES-NOT: atomic_temp
// REWRITES-NOT: cmpxchg_bool

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
// COMMON-LOWERING-NEXT: /// returns 100, a = 105
// COMMON-LOWERING-NEXT: /// returns 105, a = 95
// COMMON-LOWERING-NEXT: /// 95
// COMMON-LOWERING-NEXT: /// &
// COMMON-LOWERING-NEXT: /// 60 = 28
// COMMON-LOWERING-NEXT: /// 28 | 1 = 29
// COMMON-LOWERING-NEXT: /// 29 ^ 15 = 18
// COMMON-LOWERING-NEXT: /// returns 18, a = 7
// COMMON-LOWERING-NEXT: /// success, a = 42
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut a: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut expected: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut expected2: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .fetch_and({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .fetch_or({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 15;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     expected = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         expected = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 999;
// COMMON-LOWERING-NEXT:     expected2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         expected2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
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
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: /// returns 100, a = 105
// COMMON-REWRITES-NEXT: /// returns 105, a = 95
// COMMON-REWRITES-NEXT: /// 95
// COMMON-REWRITES-NEXT: /// &
// COMMON-REWRITES-NEXT: /// 60 = 28
// COMMON-REWRITES-NEXT: /// 28 | 1 = 29
// COMMON-REWRITES-NEXT: /// 29 ^ 15 = 18
// COMMON-REWRITES-NEXT: /// returns 18, a = 7
// COMMON-REWRITES-NEXT: /// success, a = 42
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut a: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut expected: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut expected2: i32 = 0;
// COMMON-REWRITES-NEXT:     a = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .fetch_and({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .fetch_or({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 15;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// COMMON-REWRITES-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     expected = 7;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// COMMON-REWRITES-NEXT:             expected,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         expected = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     expected2 = 999;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// COMMON-REWRITES-NEXT:             expected2,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         expected2 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
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
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
