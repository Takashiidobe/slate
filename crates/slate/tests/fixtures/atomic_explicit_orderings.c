#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store_explicit(&a, 10, memory_order_relaxed);
  int relaxed_load = atomic_load_explicit(&a, memory_order_relaxed);

  atomic_store_explicit(&a, 20, memory_order_release);
  int acquire_load = atomic_load_explicit(&a, memory_order_acquire);

  atomic_store_explicit(&a, 30, memory_order_seq_cst);
  int consume_load = atomic_load_explicit(&a, memory_order_consume);

  int old_add  = atomic_fetch_add_explicit(&a, 2, memory_order_acq_rel);
  int old_or   = atomic_fetch_or_explicit(&a, 1, memory_order_relaxed);
  int old_xchg = atomic_exchange_explicit(&a, 5, memory_order_acquire);

  int expected = 5;
  int ok       = atomic_compare_exchange_strong_explicit(
      &a, &expected, 8, memory_order_acq_rel, memory_order_acquire);

  atomic_thread_fence(memory_order_release);
  atomic_thread_fence(memory_order_acquire);
  atomic_thread_fence(memory_order_relaxed);

  printf("%d %d %d %d %d %d %d %d %d\n", relaxed_load, acquire_load,
         consume_load, old_add, old_or, old_xchg, ok, expected, (int)a);
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
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut relaxed_load: i32 = 0;
// LOWERING-NEXT:     let mut acquire_load: i32 = 0;
// LOWERING-NEXT:     let mut consume_load: i32 = 0;
// LOWERING-NEXT:     let mut old_add: i32 = 0;
// LOWERING-NEXT:     let mut old_or: i32 = 0;
// LOWERING-NEXT:     let mut old_xchg: i32 = 0;
// LOWERING-NEXT:     let mut expected: i32 = 0;
// LOWERING-NEXT:     let mut ok: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     relaxed_load = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::Release) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     acquire_load = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     consume_load = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// LOWERING-NEXT:     old_add = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_or({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     old_or = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     old_xchg = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     expected = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Result<i32, i32> = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).compare_exchange({{_v[0-9]+}}, {{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel, std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = match {{_v[0-9]+}} { Ok(v) => v, Err(v) => v };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_ok();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     if {{_v[0-9]+}} {
// LOWERING-NEXT:         expected = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     ok = {{_v[0-9]+}};
// LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Release);
// LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = relaxed_load;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = acquire_load;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = consume_load;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = old_add;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = old_or;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = old_xchg;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ok;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = expected;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: .load(std::sync::atomic::Ordering::Acquire)
// REWRITES-DAG: std::sync::atomic::fence(std::sync::atomic::Ordering::Release);
// REWRITES-DAG: std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire);
// REWRITES-NOT: fence(std::sync::atomic::Ordering::Relaxed)
