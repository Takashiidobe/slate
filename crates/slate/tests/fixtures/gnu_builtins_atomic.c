#include <stdbool.h>
#include <stdio.h>

static int gnu_builtin_atomic(void) {
  int  value = 0;
  int  expected;
  int  total = 0;
  bool flag  = false;
  __atomic_store_n(&value, 10, __ATOMIC_SEQ_CST);
  total    += __atomic_load_n(&value, __ATOMIC_SEQ_CST);
  total    += __atomic_exchange_n(&value, 20, __ATOMIC_SEQ_CST);
  expected  = 20;
  total    += __atomic_compare_exchange_n(&value, &expected, 30, false,
                                          __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_add(&value, 5, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_sub(&value, 3, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_and(&value, 31, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_or(&value, 64, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_xor(&value, 3, __ATOMIC_SEQ_CST);
  total    += __atomic_fetch_nand(&value, 127, __ATOMIC_SEQ_CST);
  value     = 10;
  total    += __atomic_add_fetch(&value, 2, __ATOMIC_SEQ_CST);
  total    += __atomic_sub_fetch(&value, 3, __ATOMIC_SEQ_CST);
  total    += __atomic_and_fetch(&value, 7, __ATOMIC_SEQ_CST);
  total    += __atomic_or_fetch(&value, 16, __ATOMIC_SEQ_CST);
  total    += __atomic_xor_fetch(&value, 3, __ATOMIC_SEQ_CST);
  total    += __atomic_nand_fetch(&value, 31, __ATOMIC_SEQ_CST);
  total    += !__atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  total    += __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  total += !flag;
  __atomic_thread_fence(__ATOMIC_SEQ_CST);
  __atomic_signal_fence(__ATOMIC_SEQ_CST);
  total += __atomic_always_lock_free(sizeof(int), &value);
  total += __atomic_is_lock_free(sizeof(int), &value);
  return total;
}

static int gnu_builtin_sync(void) {
  int value  = 10;
  int total  = 0;
  total     += __sync_fetch_and_add(&value, 2);
  total     += __sync_fetch_and_sub(&value, 3);
  total     += __sync_fetch_and_and(&value, 7);
  total     += __sync_fetch_and_or(&value, 16);
  total     += __sync_fetch_and_xor(&value, 3);
  total     += __sync_fetch_and_nand(&value, 31);
  value      = 10;
  total     += __sync_add_and_fetch(&value, 2);
  total     += __sync_sub_and_fetch(&value, 3);
  total     += __sync_bool_compare_and_swap(&value, 9, 20);
  total     += __sync_val_compare_and_swap(&value, 20, 30);
  total     += __sync_lock_test_and_set(&value, 40);
  __sync_lock_release(&value);
  total += value;
  total += __sync_swap(&value, 50);
  __sync_synchronize();
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_builtin_atomic(), gnu_builtin_sync());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_builtin_atomic() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     let mut expected: i32 = 20;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut flag: bool = false;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(10i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(20i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += (unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = core::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         match core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_0]],
// CHECK-NEXT:                 30i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             ) {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_1:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_0]] = [[T_1]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             },
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(5i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(31i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or(64i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand(127i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     value = 10;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(2i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_add(2)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_sub(3)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(7i32, core::sync::atomic::Ordering::SeqCst) & 7
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or(16i32, core::sync::atomic::Ordering::SeqCst) | 16
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor(3i32, core::sync::atomic::Ordering::SeqCst) ^ 3
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         !(core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand(31i32, core::sync::atomic::Ordering::SeqCst) & 31)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += !unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(true, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     } as i32;
// CHECK-NEXT:     total += (unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(true, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(false, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += !flag as i32;
// CHECK-NEXT:     core::sync::atomic::fence(core::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     core::sync::atomic::compiler_fence(core::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     total += true as i32;
// CHECK-NEXT:     total += true as i32;
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_builtin_sync() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 10;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(2i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(7i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or(16i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand(31i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     value = 10;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(2i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_add(2)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_sub(3)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += (unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 9i32,
// CHECK-NEXT:                 20i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:             .is_ok()
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 20i32,
// CHECK-NEXT:                 30i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_2:__t[0-9]+]]| [[T_2]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(40i32, core::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(0i32, core::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += value;
// CHECK-NEXT:     total += unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(50i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     core::sync::atomic::fence(core::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", gnu_builtin_atomic(), gnu_builtin_sync());
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
