
#include <stdio.h>
#include <time.h>

// @check-fn-begin
int main(void) {
  struct timespec value    = {0};
  int             result   = timespec_get(&value, TIME_UTC);
  int nanoseconds_in_range = value.tv_nsec >= 0 && value.tv_nsec < 1000000000L;
  printf("%d %d\n", result == TIME_UTC, nanoseconds_in_range);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: timespec = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: timespec = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tv_sec = 0 as i64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let result: i32 = unsafe { timespec_get(std::ptr::addr_of_mut!(value), 1 as i32) };
// CHECK-NEXT:     let nanoseconds_in_range: i32 = (value.tv_nsec >= 0 && value.tv_nsec < 1000000000)
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     println!("{} {}", (result == 1) as i32, nanoseconds_in_range);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
