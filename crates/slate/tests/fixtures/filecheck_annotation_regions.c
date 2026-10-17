#include <stdio.h>

// @check-fn-begin
int main(void) {
  int value;
  int lowered_value;
  int lowered_absence;
  lowered_value   = 6 * 7;
  lowered_absence = 5 + 6;
  value           = 40 + 2;
  printf("%d %d %d\n", value, lowered_value, lowered_absence);
  puts("_v9 anon_4 anon_struct_i32");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let value: i32 = 40 + 2;
// CHECK-NEXT:     let lowered_value: i32 = 6 * 7;
// CHECK-NEXT:     let lowered_absence: i32 = 5 + 6;
// CHECK-NEXT:     println!("{} {} {}", value, lowered_value, lowered_absence);
// CHECK-NEXT:     unsafe { puts(c"_v9 anon_4 anon_struct_i32".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
