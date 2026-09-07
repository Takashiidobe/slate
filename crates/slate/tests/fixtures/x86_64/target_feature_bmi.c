// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((target("bmi"))) int bmi_probe(int x) { return x + 1; }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) { return bmi_probe(41); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: #[target_feature(enable = "bmi1")]
// LOWERING-X86_64-GNU-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: #[target_feature(enable = "bmi1")]
// REWRITES-X86_64-GNU-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     {{arg[0-9]+}} + 1
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
