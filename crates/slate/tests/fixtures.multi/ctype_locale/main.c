#include <ctype.h>
#include <stdio.h>

void set_project_locale(void);

// @lowering-fn-begin
int main(void) {
  set_project_locale();
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: let mut lower: i32 = 0;
// LOWERING-DAG: let mut upper: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: set_project_locale();
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 113;
// LOWERING-DAG: lower = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 81;
// LOWERING-DAG: upper = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = lower;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = upper;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { toupper(
// REWRITES-DAG: unsafe { tolower(
// REWRITES: {{^}}}
