#include <stdio.h>

static int parameter_bound(int length, int values[length++]) {
  return length + values[0];
}

int main(void) {
  int bound = 3;
  // @lowering-begin
  // @rewrite-begin
  unsigned long evaluated = sizeof(int[bound++]);
  unsigned long unevaluated = sizeof(int(*)[bound++]);
  int values[] = {7, 8, 9};
  int parameter = parameter_bound(3, values);
  printf("%lu %lu %d %d\n", evaluated, unevaluated, bound, parameter);
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = bound;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: bound = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG: evaluated = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG: unevaluated = {{_v[0-9]+}};
// LOWERING-DAG: values = [7, 8, 9];
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = parameter_bound({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: parameter = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = evaluated;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = unevaluated;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = bound;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = parameter;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = bound;
// REWRITES-DAG: bound = {{_v[0-9]+}} + 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-DAG: evaluated = {{_v[0-9]+}} * ({{_v[0-9]+}} as u64);
// REWRITES-DAG: unevaluated = 8;
// REWRITES-DAG: values = [7, 8, 9];
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-DAG: parameter = parameter_bound({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, evaluated, unevaluated, bound, parameter) };
// SLATE-FILECHECK-END rewrites
