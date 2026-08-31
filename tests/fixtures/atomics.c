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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_sub({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 60;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_and({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_or({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 15;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_xor({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     expected = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Result<i32, i32> = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange({{_v[0-9]+}}, {{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = match {{_v[0-9]+}} { Ok(v) => v, Err(v) => v };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_ok();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     if {{_v[0-9]+}} {
// LOWERING-NEXT:         expected = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 999;
// LOWERING-NEXT:     expected2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Result<i32, i32> = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange({{_v[0-9]+}}, {{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = match {{_v[0-9]+}} { Ok(v) => v, Err(v) => v };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_ok();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     if {{_v[0-9]+}} {
// LOWERING-NEXT:         expected2 = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-NOT: _atomictmp
// REWRITES-NOT: atomic_temp
// REWRITES-NOT: cmpxchg_bool
