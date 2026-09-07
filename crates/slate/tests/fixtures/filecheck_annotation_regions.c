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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 7;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 6 * {{__v[0-9]+}};
// REWRITES-DAG: println!("{} {} {}", {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG: unsafe { puts(c"_v9 anon_4 anon_struct_i32".as_ptr()) };
// REWRITES-DAG: unsafe { fflush(std::ptr::null_mut()) };
// SLATE-FILECHECK-END rewrites
