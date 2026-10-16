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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = (40 as i32) + (2 as i32);
// CHECK-NEXT:     let mut lowered_value: i32 = (6 as i32) * (7 as i32);
// CHECK-NEXT:     let mut lowered_absence: i32 = (5 as i32) + (6 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             value,
// CHECK-NEXT:             lowered_value,
// CHECK-NEXT:             lowered_absence,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { puts(c"_v9 anon_4 anon_struct_i32".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
