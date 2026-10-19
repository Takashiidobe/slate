#include <stdio.h>

// @check-fn-begin
int main(void) {
  puts("hello");
  printf("%d\n", 42);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(c"hello".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", 42i32);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
