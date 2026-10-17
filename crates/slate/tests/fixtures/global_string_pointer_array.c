#include <stdio.h>

static char *names[] = {"alpha", "beta"};

// @check-fn-begin
int main(void) {
  printf("%s %s\n", names[0], names[1]);
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s\n".as_ptr() as *const i8,
// CHECK-NEXT:             unsafe { names.0[0] },
// CHECK-NEXT:             unsafe { names.0[1] },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
