
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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct tm {
// LOWERING-NEXT:     tm_sec: i32,
// LOWERING-NEXT:     tm_min: i32,
// LOWERING-NEXT:     tm_hour: i32,
// LOWERING-NEXT:     tm_mday: i32,
// LOWERING-NEXT:     tm_mon: i32,
// LOWERING-NEXT:     tm_year: i32,
// LOWERING-NEXT:     tm_wday: i32,
// LOWERING-NEXT:     tm_yday: i32,
// LOWERING-NEXT:     tm_isdst: i32,
// LOWERING-NEXT:     tm_gmtoff: i64,
// LOWERING-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// LOWERING-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut timestamp: i64 = 0;
// LOWERING-NEXT:     let mut utc: tm = tm {
// LOWERING-NEXT:         tm_sec: 0,
// LOWERING-NEXT:         tm_min: 0,
// LOWERING-NEXT:         tm_hour: 0,
// LOWERING-NEXT:         tm_mday: 0,
// LOWERING-NEXT:         tm_mon: 0,
// LOWERING-NEXT:         tm_year: 0,
// LOWERING-NEXT:         tm_wday: 0,
// LOWERING-NEXT:         tm_yday: 0,
// LOWERING-NEXT:         tm_isdst: 0,
// LOWERING-NEXT:         tm_gmtoff: 0,
// LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut local: tm = tm {
// LOWERING-NEXT:         tm_sec: 0,
// LOWERING-NEXT:         tm_min: 0,
// LOWERING-NEXT:         tm_hour: 0,
// LOWERING-NEXT:         tm_mday: 0,
// LOWERING-NEXT:         tm_mon: 0,
// LOWERING-NEXT:         tm_year: 0,
// LOWERING-NEXT:         tm_wday: 0,
// LOWERING-NEXT:         tm_yday: 0,
// LOWERING-NEXT:         tm_isdst: 0,
// LOWERING-NEXT:         tm_gmtoff: 0,
// LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     timestamp = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// LOWERING-NEXT:         tm_sec: 0,
// LOWERING-NEXT:         tm_min: 0,
// LOWERING-NEXT:         tm_hour: 0,
// LOWERING-NEXT:         tm_mday: 0,
// LOWERING-NEXT:         tm_mon: 0,
// LOWERING-NEXT:         tm_year: 0,
// LOWERING-NEXT:         tm_wday: 0,
// LOWERING-NEXT:         tm_yday: 0,
// LOWERING-NEXT:         tm_isdst: 0,
// LOWERING-NEXT:         tm_gmtoff: 0,
// LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     utc = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// LOWERING-NEXT:         tm_sec: 0,
// LOWERING-NEXT:         tm_min: 0,
// LOWERING-NEXT:         tm_hour: 0,
// LOWERING-NEXT:         tm_mday: 0,
// LOWERING-NEXT:         tm_mon: 0,
// LOWERING-NEXT:         tm_year: 0,
// LOWERING-NEXT:         tm_wday: 0,
// LOWERING-NEXT:         tm_yday: 0,
// LOWERING-NEXT:         tm_isdst: 0,
// LOWERING-NEXT:         tm_gmtoff: 0,
// LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     local = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// LOWERING-NEXT:         gmtime_r(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// LOWERING-NEXT:         localtime_r(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

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
// REWRITES-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
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
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// REWRITES-NEXT:         gmtime_r(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// REWRITES-NEXT:         localtime_r(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
