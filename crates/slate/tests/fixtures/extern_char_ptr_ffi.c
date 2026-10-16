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
// CHECK-NEXT:     let msg: *const i8 = c"hello".as_ptr() as *const i8;
// CHECK-NEXT:     let n: u64 = unsafe { "hello".len() as u64 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(msg) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr() as *const i8, n) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
