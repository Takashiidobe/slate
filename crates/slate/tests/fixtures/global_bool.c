#include <stdbool.h>
#include <stdio.h>

bool        enabled = true;
static bool disabled;

// @check-fn-begin
int main(void) {
  printf("%d %d\n", enabled, disabled);
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             (unsafe { enabled }) as i32,
// CHECK-NEXT:             (unsafe { disabled }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
