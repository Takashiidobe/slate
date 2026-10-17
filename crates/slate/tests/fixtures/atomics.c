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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .store(100 as i32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let loaded: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut fa: i32 = 0;
// CHECK-NEXT:     fa = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = 5 as i32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 100, a = 105
// CHECK-NEXT:     let mut fs: i32 = 0;
// CHECK-NEXT:     fs = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = 10 as i32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_sub([[T_2]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 105, a = 95
// CHECK-NEXT:     let mut fand: i32 = 0;
// CHECK-NEXT:     fand = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = 60 as i32;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_and([[T_4]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // 95 & 60 = 28
// CHECK-NEXT:     let mut forr: i32 = 0;
// CHECK-NEXT:     forr = unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_or([[T_6]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // 28 | 1 = 29
// CHECK-NEXT:     let mut fxor: i32 = 0;
// CHECK-NEXT:     fxor = unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = 15 as i32;
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_xor([[T_8]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_9]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // 29 ^ 15 = 18
// CHECK-NEXT:     let mut xchg_old: i32 = 0;
// CHECK-NEXT:     xchg_old = unsafe {
// CHECK-NEXT:         let [[T_10:__t[0-9]+]] = 7 as i32;
// CHECK-NEXT:         let [[T_11:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .swap([[T_10]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_11]]
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 18, a = 7
// CHECK-NEXT:     let mut expected: i32 = 7;
// CHECK-NEXT:     let mut ok: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_12:__t[0-9]+]] = std::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         let [[T_13:__t[0-9]+]] = 42 as i32;
// CHECK-NEXT:         let [[T_14:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_12]],
// CHECK-NEXT:                 [[T_13]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         match [[T_14]] {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_15:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_12]] = [[T_15]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     ok = [[V_0]] as i32; // success, a = 42
// CHECK-NEXT:     let mut expected2: i32 = 999;
// CHECK-NEXT:     let mut bad: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_16:__t[0-9]+]] = std::ptr::addr_of_mut!(expected2);
// CHECK-NEXT:         let [[T_17:__t[0-9]+]] = 0 as i32;
// CHECK-NEXT:         let [[T_18:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_16]],
// CHECK-NEXT:                 [[T_17]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         match [[T_18]] {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_19:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_16]] = [[T_19]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     bad = [[V_1]] as i32; // fail, a stays 42
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {} {} {} {}",
// CHECK-NEXT:         loaded,
// CHECK-NEXT:         fa,
// CHECK-NEXT:         fs,
// CHECK-NEXT:         fand,
// CHECK-NEXT:         forr,
// CHECK-NEXT:         fxor,
// CHECK-NEXT:         xchg_old,
// CHECK-NEXT:         ok,
// CHECK-NEXT:         expected,
// CHECK-NEXT:         bad,
// CHECK-NEXT:         expected2,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(a) as *mut i32)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
