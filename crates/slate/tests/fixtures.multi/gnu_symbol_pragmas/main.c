#include <stdio.h>

extern int pragma_weak_target(int);

extern int pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
extern int pragma_renamed(int);

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  printf("%d %d\n", pragma_weak_alias(29), pragma_renamed(13));
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 29;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = pragma_weak_alias({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 13;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = pragma_actual({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     println!("{} {}", pragma_weak_alias(29), pragma_actual(13));
// REWRITES-DAG:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
