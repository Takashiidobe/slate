// { dg-do run { target x86_64-*-* } }
// @lowering-fn-begin
// @rewrite-fn-begin
__attribute__((target("bmi"))) int bmi_probe(int x) { return x + 1; }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) { return bmi_probe(41); }

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: /// { dg-do run { target x86_64-*-* } }
// COMMON-LOWERING-DAG: #[target_feature(enable = "bmi1")]
// COMMON-LOWERING-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: /// { dg-do run { target x86_64-*-* } }
// COMMON-REWRITES-DAG: #[target_feature(enable = "bmi1")]
// COMMON-REWRITES-DAG: unsafe fn bmi_probe({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     {{arg[0-9]+}} + 1
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
