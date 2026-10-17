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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_atomic() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     let mut expected: i32 = 20;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut flag: bool = false as bool;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value) as *mut i32)
// CHECK-NEXT:             .store(10 as i32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_0]]
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = 20 as i32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_1]] + [[V_2]];
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = std::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = 30 as i32;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_2]],
// CHECK-NEXT:                 [[T_3]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         match [[T_4]] {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_5:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_2]] = [[T_5]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_3]] + ([[V_4]] as i32);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = 5 as i32;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_6]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_5]] + [[V_6]];
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_8]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_9]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_7]] + [[V_8]];
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_10:__t[0-9]+]] = 31 as i32;
// CHECK-NEXT:         let [[T_11:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and([[T_10]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_11]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_9]] + [[V_10]];
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_12:__t[0-9]+]] = 64 as i32;
// CHECK-NEXT:         let [[T_13:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_12]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_13]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_11]] + [[V_12]];
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_14:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:         let [[T_15:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_14]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_15]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_13]] + [[V_14]];
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_16:__t[0-9]+]] = 127 as i32;
// CHECK-NEXT:         let [[T_17:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand([[T_16]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_17]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_15]] + [[V_16]];
// CHECK-NEXT:     value = 10;
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_18:__t[0-9]+]] = 2 as i32;
// CHECK-NEXT:         let [[T_19:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_18]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_19]].wrapping_add([[T_18]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_17]] + [[V_18]];
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_20:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:         let [[T_21:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_20]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_21]].wrapping_sub([[T_20]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_19]] + [[V_20]];
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_22:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_22:__t[0-9]+]] = 7 as i32;
// CHECK-NEXT:         let [[T_23:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and([[T_22]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_23]] & [[T_22]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_21]] + [[V_22]];
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_24:__t[0-9]+]] = 16 as i32;
// CHECK-NEXT:         let [[T_25:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_24]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_25]] | [[T_24]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_23]] + [[V_24]];
// CHECK-NEXT:     let [[V_25:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_26:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_26:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:         let [[T_27:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_26]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_27]] ^ [[T_26]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_25]] + [[V_26]];
// CHECK-NEXT:     let [[V_27:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_28:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_28:__t[0-9]+]] = 31 as i32;
// CHECK-NEXT:         let [[T_29:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand([[T_28]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         !([[T_29]] & [[T_28]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_27]] + [[V_28]];
// CHECK-NEXT:     let [[V_29:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_30:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_30:__t[0-9]+]] = true as bool;
// CHECK-NEXT:         let [[T_31:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_30]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_31]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_31:__v[0-9]+]]: i32 = [[V_29]] + (![[V_30]] as i32);
// CHECK-NEXT:     total = [[V_31]];
// CHECK-NEXT:     let [[V_32:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_33:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_32:__t[0-9]+]] = true as bool;
// CHECK-NEXT:         let [[T_33:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_32]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_33]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_32]] + ([[V_33]] as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(false as bool, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_34:__v[0-9]+]]: i32 = total + (!flag as i32);
// CHECK-NEXT:     total = [[V_34]];
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     std::sync::atomic::compiler_fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     total += (true as bool) as i32;
// CHECK-NEXT:     total += (true as bool) as i32;
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_sync() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 10;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_35:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_36:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_0]] = 2 as i32;
// CHECK-NEXT:         let [[T_1]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_35]] + [[V_36]];
// CHECK-NEXT:     let [[V_37:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_38:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_3]] = 3 as i32;
// CHECK-NEXT:         let [[T_2]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_3]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_37]] + [[V_38]];
// CHECK-NEXT:     let [[V_39:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_40:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_5]] = 7 as i32;
// CHECK-NEXT:         let [[T_4]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and([[T_5]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_39]] + [[V_40]];
// CHECK-NEXT:     let [[V_41:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_42:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_6]] = 16 as i32;
// CHECK-NEXT:         let [[T_7]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_6]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_41]] + [[V_42]];
// CHECK-NEXT:     let [[V_43:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_44:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_8]] = 3 as i32;
// CHECK-NEXT:         let [[T_9]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_8]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_9]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_43]] + [[V_44]];
// CHECK-NEXT:     let [[V_45:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_46:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_10]] = 31 as i32;
// CHECK-NEXT:         let [[T_11]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand([[T_10]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_11]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_45]] + [[V_46]];
// CHECK-NEXT:     value = 10;
// CHECK-NEXT:     let [[V_47:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_48:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_12]] = 2 as i32;
// CHECK-NEXT:         let [[T_13]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_12]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_13]].wrapping_add([[T_12]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_47]] + [[V_48]];
// CHECK-NEXT:     let [[V_49:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_50:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_14]] = 3 as i32;
// CHECK-NEXT:         let [[T_15]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_14]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_15]].wrapping_sub([[T_14]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_49]] + [[V_50]];
// CHECK-NEXT:     let [[V_51:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_52:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_17]] = 9 as i32;
// CHECK-NEXT:         let [[T_16]] = 20 as i32;
// CHECK-NEXT:         let [[T_19]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_17]],
// CHECK-NEXT:                 [[T_16]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         [[T_19]].is_ok()
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_51]] + ([[V_52]] as i32);
// CHECK-NEXT:     let [[V_53:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_54:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_21]] = 20 as i32;
// CHECK-NEXT:         let [[T_20]] = 30 as i32;
// CHECK-NEXT:         let [[T_23]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_21]],
// CHECK-NEXT:                 [[T_20]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         [[T_23]].unwrap_or_else(|[[T_22]]| [[T_22]])
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_53]] + [[V_54]];
// CHECK-NEXT:     let [[V_55:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_56:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_24]] = 40 as i32;
// CHECK-NEXT:         let [[T_25]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_24]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_25]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_55]] + [[V_56]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value) as *mut i32)
// CHECK-NEXT:             .store(0 as i32, std::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     total += value;
// CHECK-NEXT:     let [[V_57:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     let [[V_58:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_26]] = 50 as i32;
// CHECK-NEXT:         let [[T_27]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_26]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_27]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total = [[V_57]] + [[V_58]];
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", gnu_builtin_atomic(), gnu_builtin_sync());
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
