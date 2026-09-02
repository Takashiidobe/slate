// @lowering-fn-begin
// @rewrite-fn-begin
int match(int value) { return value; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C" fn r#match({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     return {{arg[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn r#match({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     return {{arg[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
