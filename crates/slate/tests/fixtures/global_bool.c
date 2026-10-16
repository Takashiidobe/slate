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
// CHECK-NEXT:     println!("{} {}", (unsafe { enabled }) as i32, (unsafe { disabled }) as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
