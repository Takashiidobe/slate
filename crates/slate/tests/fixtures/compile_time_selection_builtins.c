#include <stdio.h>

#define TYPE_NAME(x)                                                           \
  _Generic((x), int: "int", double: "double", default: "other")

// @check-fn-begin
int main(void) {
  int a = 3, b = 4;
  int chosen_true  = __builtin_choose_expr(1, a, b);
  int chosen_false = __builtin_choose_expr(0, a, b);

  int same_type = __builtin_types_compatible_p(int, int);
  int diff_type = __builtin_types_compatible_p(int, float);

  int         i      = 7;
  double      d      = 2.5;
  const char *i_name = TYPE_NAME(i);
  const char *d_name = TYPE_NAME(d);

  printf("%d %d %d %d %s %s\n", chosen_true, chosen_false, same_type, diff_type,
         i_name, d_name);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = 3 as i32;
// CHECK-NEXT:     let b: i32 = 4 as i32;
// CHECK-NEXT:     let chosen_true: i32 = a;
// CHECK-NEXT:     let chosen_false: i32 = b;
// CHECK-NEXT:     let same_type: i32 = 1 as i32;
// CHECK-NEXT:     let diff_type: i32 = 0 as i32;
// CHECK-NEXT:     let i_name: *const i8 = c"int".as_ptr() as *const i8;
// CHECK-NEXT:     let d_name: *const i8 = c"double".as_ptr() as *const i8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %s %s\n".as_ptr() as *const i8,
// CHECK-NEXT:             chosen_true,
// CHECK-NEXT:             chosen_false,
// CHECK-NEXT:             same_type,
// CHECK-NEXT:             diff_type,
// CHECK-NEXT:             i_name,
// CHECK-NEXT:             d_name,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
