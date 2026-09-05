
#include <stdio.h>
#include <time.h>

// @rewrite-fn-begin
int main(void) {
  struct timespec value    = {0};
  int             result   = timespec_get(&value, TIME_UTC);
  int nanoseconds_in_range = value.tv_nsec >= 0 && value.tv_nsec < 1000000000L;
  printf("%d %d\n", result == TIME_UTC, nanoseconds_in_range);
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut value: libc::timespec = libc::timespec {
// COMMON-REWRITES-DAG:         tv_sec: 0,
// COMMON-REWRITES-DAG:         tv_nsec: 0,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         timespec_get(
// COMMON-REWRITES-DAG:             std::ptr::addr_of_mut!(value) as *mut libc::timespec,
// COMMON-REWRITES-DAG:             1 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if value.tv_nsec >= 0 {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = value.tv_nsec < 1000000000;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), ({{__v[0-9]+}} == 1) as i32, {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
