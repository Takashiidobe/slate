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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .store(10 as i32, std::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     let relaxed_load: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .store(20 as i32, std::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     let acquire_load: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .store(30 as i32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let consume_load: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_add: i32 = 0;
// CHECK-NEXT:     old_add = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = 2 as i32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::AcqRel);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_or: i32 = 0;
// CHECK-NEXT:     old_or = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_2]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old_xchg: i32 = 0;
// CHECK-NEXT:     old_xchg = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = 5 as i32;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_4]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut expected: i32 = 5;
// CHECK-NEXT:     let mut ok: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = std::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = 8 as i32;
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_6]],
// CHECK-NEXT:                 [[T_7]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::AcqRel,
// CHECK-NEXT:                 std::sync::atomic::Ordering::Acquire,
// CHECK-NEXT:             );
// CHECK-NEXT:         match [[T_8]] {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_9:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_6]] = [[T_9]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     ok = [[V_0]] as i32;
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Release);
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:     {};
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {}", relaxed_load, acquire_load, consume_load, old_add,
// CHECK-NEXT:         old_or, old_xchg, ok, expected, unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(a) as * mut i32)
// CHECK-NEXT:         .load(std::sync::atomic::Ordering::SeqCst) }
// CHECK-NEXT:     );
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
