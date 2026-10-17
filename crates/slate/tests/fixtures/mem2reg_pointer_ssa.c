#include <stdio.h>

int  value         = 7;
int *value_pointer = &value;
int  values[]      = {11, 13, 17};

int *return_global(void) { return &value; }

int **return_global_pointer(void) { return &value_pointer; }

int *return_element(void) { return &values[1]; }

// @check-fn-begin
int main(void) {
  printf("%d\n",
         *return_global() + **return_global_pointer() + *return_element());
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (unsafe { * return_global() }) + unsafe { * unsafe { *
// CHECK-NEXT:         return_global_pointer() } } + unsafe { * return_element() }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
