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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .store(100i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let loaded: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .load(core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 100, a = 105
// CHECK-NEXT:     let fa: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_add(5i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 105, a = 95
// CHECK-NEXT:     let fs: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_sub(10i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // 95 & 60 = 28
// CHECK-NEXT:     let fand: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_and(60i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // 28 | 1 = 29
// CHECK-NEXT:     let forr: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_or(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // 29 ^ 15 = 18
// CHECK-NEXT:     let fxor: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .fetch_xor(15i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     // returns 18, a = 7
// CHECK-NEXT:     let xchg_old: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of_mut!(a) as *mut i32)
// CHECK-NEXT:             .swap(7i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut expected: i32 = 7;
// CHECK-NEXT:     // success, a = 42
// CHECK-NEXT:     let ok: i32 = (unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = core::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         match core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_0]],
// CHECK-NEXT:                 42i32,
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
// CHECK-NEXT:     let mut expected2: i32 = 999;
// CHECK-NEXT:     // fail, a stays 42
// CHECK-NEXT:     let bad: i32 = (unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = core::ptr::addr_of_mut!(expected2);
// CHECK-NEXT:         match core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(a) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_2]],
// CHECK-NEXT:                 0i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             ) {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_3:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_2]] = [[T_3]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             },
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     core::sync::atomic::fence(core::sync::atomic::Ordering::SeqCst);
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
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of!(a) as *mut i32)
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
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
