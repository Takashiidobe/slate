#include <stdio.h>

int main(void) {
  int value;
  int lowered_value;
  int lowered_absence;
  // @lowering-begin
  // @rewrite-begin
  lowered_value   = 6 * 7;
  // @rewrite-end
  // @lowering-end
  lowered_absence = 5 + 6;
  value           = 40 + 2;
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
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 6 * {{_v[0-9]+}};
// REWRITES-DAG: unsafe { printf(c"%d %d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-DAG: unsafe { puts(c"_v9 anon_4 anon_struct_i32".as_ptr()) };
// SLATE-FILECHECK-END rewrites
