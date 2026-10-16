#include <stdio.h>
#include <string.h>

// @check-fn-begin
int main(void) {
  const char *msg = "hello";
  size_t      n   = strlen(msg);
  puts(msg);
  printf("%zu\n", n);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut msg: *const i8 = c"hello".as_ptr() as *const i8;
// CHECK-NEXT:     let mut n: u64 = unsafe { strlen(msg) };
// CHECK-NEXT:     unsafe { puts(msg) };
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr() as *const i8, n) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
