#include <stdio.h>

// @check-fn-begin
int main(void) {
  printf("%d %d\n", 7, 8);
  printf("%ld\n", 42L);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", 7 as i32, 8 as i32);
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), 42 as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
