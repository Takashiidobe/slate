int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  int result;
  result = alias_global;
  return result;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { real_global };
// LOWERING-DAG:     return std::process::ExitCode::from({{__v[0-9]+}} as u8);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     return std::process::ExitCode::from((unsafe { real_global }) as u8);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
