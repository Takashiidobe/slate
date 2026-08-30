// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((target("bmi"))) int bmi_probe(int x) {
  return x + 1;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  return bmi_probe(41);
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[target_feature(enable = "bmi1")]
// LOWERING-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let mut x: i32 = 0;
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: x = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[target_feature(enable = "bmi1")]
// REWRITES-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: __retval = x + {{_v[0-9]+}};
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
