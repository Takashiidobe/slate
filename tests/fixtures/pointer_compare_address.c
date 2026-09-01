
// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: _v{{[0-9]+}} == std::ptr::addr_of_mut!(utc)
// LOWERING-DAG: _v{{[0-9]+}} == std::ptr::addr_of_mut!(local)
// LOWERING-NOT: _v{{[0-9]+}} == utc
// LOWERING-NOT: _v{{[0-9]+}} == local
// LOWERING: {{^}}}

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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct tm {
// REWRITES-NEXT:     tm_sec: i32,
// REWRITES-NEXT:     tm_min: i32,
// REWRITES-NEXT:     tm_hour: i32,
// REWRITES-NEXT:     tm_mday: i32,
// REWRITES-NEXT:     tm_mon: i32,
// REWRITES-NEXT:     tm_year: i32,
// REWRITES-NEXT:     tm_wday: i32,
// REWRITES-NEXT:     tm_yday: i32,
// REWRITES-NEXT:     tm_isdst: i32,
// REWRITES-NEXT:     tm_gmtoff: i64,
// REWRITES-NEXT:     tm_zone: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut timestamp: i64 = 0;
// REWRITES-NEXT:     let mut utc: tm = tm {
// REWRITES-NEXT:         tm_sec: 0,
// REWRITES-NEXT:         tm_min: 0,
// REWRITES-NEXT:         tm_hour: 0,
// REWRITES-NEXT:         tm_mday: 0,
// REWRITES-NEXT:         tm_mon: 0,
// REWRITES-NEXT:         tm_year: 0,
// REWRITES-NEXT:         tm_wday: 0,
// REWRITES-NEXT:         tm_yday: 0,
// REWRITES-NEXT:         tm_isdst: 0,
// REWRITES-NEXT:         tm_gmtoff: 0,
// REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut local: tm = tm {
// REWRITES-NEXT:         tm_sec: 0,
// REWRITES-NEXT:         tm_min: 0,
// REWRITES-NEXT:         tm_hour: 0,
// REWRITES-NEXT:         tm_mday: 0,
// REWRITES-NEXT:         tm_mon: 0,
// REWRITES-NEXT:         tm_year: 0,
// REWRITES-NEXT:         tm_wday: 0,
// REWRITES-NEXT:         tm_yday: 0,
// REWRITES-NEXT:         tm_isdst: 0,
// REWRITES-NEXT:         tm_gmtoff: 0,
// REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     timestamp = 0;
// REWRITES-NEXT:     utc = tm {
// REWRITES-NEXT:         tm_sec: 0,
// REWRITES-NEXT:         tm_min: 0,
// REWRITES-NEXT:         tm_hour: 0,
// REWRITES-NEXT:         tm_mday: 0,
// REWRITES-NEXT:         tm_mon: 0,
// REWRITES-NEXT:         tm_year: 0,
// REWRITES-NEXT:         tm_wday: 0,
// REWRITES-NEXT:         tm_yday: 0,
// REWRITES-NEXT:         tm_isdst: 0,
// REWRITES-NEXT:         tm_gmtoff: 0,
// REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     local = tm {
// REWRITES-NEXT:         tm_sec: 0,
// REWRITES-NEXT:         tm_min: 0,
// REWRITES-NEXT:         tm_hour: 0,
// REWRITES-NEXT:         tm_mday: 0,
// REWRITES-NEXT:         tm_mon: 0,
// REWRITES-NEXT:         tm_year: 0,
// REWRITES-NEXT:         tm_wday: 0,
// REWRITES-NEXT:         tm_yday: 0,
// REWRITES-NEXT:         tm_isdst: 0,
// REWRITES-NEXT:         tm_gmtoff: 0,
// REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut tm = unsafe {
// REWRITES-NEXT:         gmtime_r(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut tm = unsafe {
// REWRITES-NEXT:         localtime_r(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(local);
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
