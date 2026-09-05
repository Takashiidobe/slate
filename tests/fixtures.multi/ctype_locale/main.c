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

// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { toupper(
// REWRITES-DAG: unsafe { tolower(
// REWRITES: {{^}}}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     set_project_locale();
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 113;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 81;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { toupper({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { tolower({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering
