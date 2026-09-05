#include <stdio.h>

#define TYPE_NAME(x)                                                           \
  _Generic((x), int: "int", double: "double", default: "other")

// @lowering-fn-begin
// @rewrite-fn-begin
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
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 2.5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         printf(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"int\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"double\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %s %s\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"int\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"double\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %s %s\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%d %d %d %d %s %s\n".as_ptr(),
// COMMON-REWRITES-DAG:             3 as i32,
// COMMON-REWRITES-DAG:             4 as i32,
// COMMON-REWRITES-DAG:             1 as i32,
// COMMON-REWRITES-DAG:             0 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:             c"int".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:             c"double".as_ptr() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:             c"int".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:             c"double".as_ptr() as *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
