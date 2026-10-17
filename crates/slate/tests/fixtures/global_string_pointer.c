#include <stdio.h>

char *bom_original = "Hi";

// @check-fn-begin
int main(void) {
  printf("%c %s\n", *bom_original, bom_original);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %s\n".as_ptr(),
// CHECK-NEXT:             (unsafe { *unsafe { bom_original } }) as i32,
// CHECK-NEXT:             unsafe { bom_original },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
