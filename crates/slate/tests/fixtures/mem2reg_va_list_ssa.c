#include <stdarg.h>
#include <stdio.h>

int consume(va_list arguments) { return va_arg(arguments, int); }

// @check-fn-begin
int relay(int count, ...) {
  va_list arguments;
  va_start(arguments, count);
  int value = consume(arguments);
  va_end(arguments);
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d\n", relay(1, 37));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}unsafe extern "C-unwind" fn relay(count: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut arguments: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     arguments = __va_args.clone();
// CHECK-NEXT:     let value: i32 = consume(arguments.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
