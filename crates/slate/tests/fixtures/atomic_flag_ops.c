#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_flag flag = ATOMIC_FLAG_INIT;

  int first = atomic_flag_test_and_set(&flag);
  atomic_flag_clear_explicit(&flag, memory_order_release);
  int second = atomic_flag_test_and_set_explicit(&flag, memory_order_acquire);
  int third  = atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed);
  atomic_flag_clear(&flag);
  int fourth = atomic_flag_test_and_set(&flag);

  printf("%d %d %d %d\n", first, second, third, fourth);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct atomic_flag {
// CHECK-NEXT:     _Value: bool,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flag: atomic_flag = atomic_flag {
// CHECK-NEXT:         _Value: 0i32 != 0i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut first: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = 1i32 != 0i32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     first = [[V_0]] as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(0i32 != 0i32, std::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut second: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = 1i32 != 0i32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_2]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     second = [[V_1]] as i32;
// CHECK-NEXT:     let mut third: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = 1i32 != 0i32;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_4]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     third = [[V_2]] as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(0i32 != 0i32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut fourth: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = 1i32 != 0i32;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!((* std::ptr::addr_of_mut!(flag))._Value)
// CHECK-NEXT:                     as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_6]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     fourth = [[V_3]] as i32;
// CHECK-NEXT:     println!("{} {} {} {}", first, second, third, fourth);
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
