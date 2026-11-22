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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let msg: &core::ffi::CStr = c"hello";
// CHECK-NEXT:     let n: u64 = msg.count_bytes() as u64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::puts(msg.as_ptr() as *const i8) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu\n".as_ptr(), n) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
