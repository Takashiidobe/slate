
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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut value: libc::timespec = libc::timespec {
// REWRITES-DAG:         tv_sec: 0,
// REWRITES-DAG:         tv_nsec: 0,
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         timespec_get(
// REWRITES-DAG:             std::ptr::addr_of_mut!(value) as *mut libc::timespec,
// REWRITES-DAG:             1 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if value.tv_nsec >= 0 {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = value.tv_nsec < 1000000000;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), ({{__v[0-9]+}} == 1) as i32, {{__v[0-9]+}} as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
