
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut timestamp: i64 = 0;
// REWRITES-NEXT: let mut utc: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: timestamp = 0;
// REWRITES-NEXT: utc = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut tm = unsafe { gmtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(utc) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut tm = unsafe { localtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(local) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(local);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

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
