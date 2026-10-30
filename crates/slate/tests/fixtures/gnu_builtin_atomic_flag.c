#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool flag   = false;
  int  first  = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  int  second = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  printf("%d %d %d\n", first, second, !flag);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flag: bool = false;
// CHECK-NEXT:     let mut first: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = true;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     first = [[V_0]] as i32;
// CHECK-NEXT:     let mut second: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = true;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_2]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     second = [[V_1]] as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(false, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {}", first, second, ! flag as i32);
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
