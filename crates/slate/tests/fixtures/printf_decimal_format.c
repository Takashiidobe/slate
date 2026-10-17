#include <stdio.h>

// @check-fn-begin
int main(void) {
  int a = 2;
  int b = 3;
  printf("sum: %d and %d\n", a, b);
  printf("hex: %x\n", a);
  printf("no newline: %d", a + b);
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = 2;
// CHECK-NEXT:     let b: i32 = 3;
// CHECK-NEXT:     println!("sum: {} and {}", a, b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"hex: %x\n".as_ptr(), a) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     print!("no newline: {}", a + b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
