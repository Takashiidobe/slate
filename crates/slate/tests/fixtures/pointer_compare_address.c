

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t    timestamp    = 0;
  struct tm utc          = {};
  struct tm local        = {};
  int       utc_result   = gmtime_r(&timestamp, &utc) == &utc;
  int       local_result = localtime_r(&timestamp, &local) == &local;
  printf("%d %d\n", utc_result, local_result);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn gmtime_r(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *const i64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *mut slate_support::tm,
// CHECK-NEXT:     ) -> *mut slate_support::tm;
// CHECK-NEXT:     fn localtime_r(
// CHECK-NEXT:         [[ARG_0]]: *const i64,
// CHECK-NEXT:         [[ARG_1]]: *mut slate_support::tm,
// CHECK-NEXT:     ) -> *mut slate_support::tm;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut timestamp: i64 = 0;
// CHECK-NEXT:     let mut utc: slate_support::tm = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: slate_support::tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local: slate_support::tm = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: slate_support::tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let utc_result: i32 = ((unsafe {
// CHECK-NEXT:         gmtime_r(
// CHECK-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(utc),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == std::ptr::addr_of_mut!(utc)) as i32;
// CHECK-NEXT:     let local_result: i32 = ((unsafe {
// CHECK-NEXT:         localtime_r(
// CHECK-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(local),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == std::ptr::addr_of_mut!(local)) as i32;
// CHECK-NEXT:     println!("{} {}", utc_result, local_result);
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
