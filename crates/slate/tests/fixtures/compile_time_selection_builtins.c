#include <stdio.h>

#define TYPE_NAME(x) \
  _Generic((x), int: "int", double: "double", default: "other")

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  int a = 3, b = 4;
  int chosen_true  = __builtin_choose_expr(1, a, b);
  int chosen_false = __builtin_choose_expr(0, a, b);

  int same_type = __builtin_types_compatible_p(int, int);
  int diff_type = __builtin_types_compatible_p(int, float);

  int    i = 7;
  double d = 2.5;
  const char *i_name = TYPE_NAME(i);
  const char *d_name = TYPE_NAME(d);

  printf("%d %d %d %d %s %s\n", chosen_true, chosen_false, same_type,
         diff_type, i_name, d_name);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 2.5;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"int\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"double\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %s %s\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         printf(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %d %d %d %s %s\n".as_ptr(),
// REWRITES-DAG:             3 as i32,
// REWRITES-DAG:             4 as i32,
// REWRITES-DAG:             1 as i32,
// REWRITES-DAG:             0 as i32,
// REWRITES-DAG:             c"int".as_ptr() as *mut i8,
// REWRITES-DAG:             c"double".as_ptr() as *mut i8,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
