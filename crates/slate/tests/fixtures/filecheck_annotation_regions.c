#include <stdio.h>

int main(void) {
  int value;
  int lowered_value;
  int lowered_absence;
  // @lowering-begin
  // @rewrite-begin
  lowered_value = 6 * 7;
  // @rewrite-end
  // @lowering-end
  lowered_absence = 5 + 6;
  value = 40 + 2;
  // @rewrite-begin
  printf("%d %d %d\n", value, lowered_value, lowered_absence);
  // @rewrite-end
  // @rewrite-begin
  puts("_v9 anon_4 anon_struct_i32");
  // @rewrite-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 6;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 7;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"_v9 anon_4 anon_struct_i32\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// SLATE-FILECHECK-END rewrites
