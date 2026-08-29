int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));

int main(void) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  result = alias_global;
  // @rewrite-end
  // @lowering-end
  return result;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { real_global };
// LOWERING-DAG: result = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: result = unsafe { real_global };
// SLATE-FILECHECK-END rewrites
