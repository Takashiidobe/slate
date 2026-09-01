int root_helper(void);

// @lowering-fn-begin
// @rewrite-fn-begin
int call_root_helper(void) { return root_helper(); }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C" fn call_root_helper() -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = root_helper();
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn call_root_helper() -> i32 {
// REWRITES-DAG: return root_helper();
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
