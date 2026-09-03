// @lowering-fn-begin
// @rewrite-fn-begin
int π(int value) { return value * 2 + 1; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(export_name = "\u{3c0}")]
// LOWERING-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(export_name = "\u{3c0}")]
// REWRITES-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     {{arg[0-9]+}} * 2 + {{_v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
