
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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct tm {
// COMMON-LOWERING-NEXT:     tm_sec: i32,
// COMMON-LOWERING-NEXT:     tm_min: i32,
// COMMON-LOWERING-NEXT:     tm_hour: i32,
// COMMON-LOWERING-NEXT:     tm_mday: i32,
// COMMON-LOWERING-NEXT:     tm_mon: i32,
// COMMON-LOWERING-NEXT:     tm_year: i32,
// COMMON-LOWERING-NEXT:     tm_wday: i32,
// COMMON-LOWERING-NEXT:     tm_yday: i32,
// COMMON-LOWERING-NEXT:     tm_isdst: i32,
// COMMON-LOWERING-NEXT:     tm_gmtoff: i64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-LOWERING-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut timestamp: i64 = 0;
// COMMON-LOWERING-NEXT:     let mut utc: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut local: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     timestamp = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     utc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     local = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-LOWERING-NEXT:         gmtime_r(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-LOWERING-NEXT:         localtime_r(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct tm {
// COMMON-REWRITES-NEXT:     tm_sec: i32,
// COMMON-REWRITES-NEXT:     tm_min: i32,
// COMMON-REWRITES-NEXT:     tm_hour: i32,
// COMMON-REWRITES-NEXT:     tm_mday: i32,
// COMMON-REWRITES-NEXT:     tm_mon: i32,
// COMMON-REWRITES-NEXT:     tm_year: i32,
// COMMON-REWRITES-NEXT:     tm_wday: i32,
// COMMON-REWRITES-NEXT:     tm_yday: i32,
// COMMON-REWRITES-NEXT:     tm_isdst: i32,
// COMMON-REWRITES-NEXT:     tm_gmtoff: i64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-REWRITES-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut timestamp: i64 = 0;
// COMMON-REWRITES-NEXT:     let mut utc: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut local: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     timestamp = 0;
// COMMON-REWRITES-NEXT:     utc = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     local = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-REWRITES-NEXT:         gmtime_r(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-REWRITES-NEXT:         localtime_r(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
