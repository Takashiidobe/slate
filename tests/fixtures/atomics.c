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
// LOWERING-NEXT: /// returns 100, a = 105
// LOWERING-NEXT: /// returns 105, a = 95
// LOWERING-NEXT: /// 95
// LOWERING-NEXT: /// &
// LOWERING-NEXT: /// 60 = 28
// LOWERING-NEXT: /// 28 | 1 = 29
// LOWERING-NEXT: /// 29 ^ 15 = 18
// LOWERING-NEXT: /// returns 18, a = 7
// LOWERING-NEXT: /// success, a = 42
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut expected: i32 = 0;
// LOWERING-NEXT:     let mut expected2: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .fetch_and({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .fetch_or({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 15;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     expected = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// LOWERING-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-NEXT:     if {{__v[0-9]+}} {
// LOWERING-NEXT:         expected = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 999;
// LOWERING-NEXT:     expected2 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// LOWERING-NEXT:             std::sync::atomic::Ordering::SeqCst,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// LOWERING-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-NEXT:     if {{__v[0-9]+}} {
// LOWERING-NEXT:         expected2 = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
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
// REWRITES-NEXT: /// returns 100, a = 105
// REWRITES-NEXT: /// returns 105, a = 95
// REWRITES-NEXT: /// 95
// REWRITES-NEXT: /// &
// REWRITES-NEXT: /// 60 = 28
// REWRITES-NEXT: /// 28 | 1 = 29
// REWRITES-NEXT: /// 29 ^ 15 = 18
// REWRITES-NEXT: /// returns 18, a = 7
// REWRITES-NEXT: /// success, a = 42
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut a: i32 = 0;
// REWRITES-NEXT:     let mut expected: i32 = 0;
// REWRITES-NEXT:     let mut expected2: i32 = 0;
// REWRITES-NEXT:     a = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .fetch_sub({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .fetch_and({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .fetch_or({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 15;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .fetch_xor({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
// REWRITES-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     expected = 7;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// REWRITES-NEXT:             expected,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// REWRITES-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         expected = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     expected2 = 999;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: Result<i32, i32> = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange(
// REWRITES-NEXT:             expected2,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-NEXT:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = match {{__v[0-9]+}} {
// REWRITES-NEXT:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-NEXT:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         expected2 = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = expected;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = expected2;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a))
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
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
