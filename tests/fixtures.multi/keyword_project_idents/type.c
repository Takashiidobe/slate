// @lowering-fn-begin
// @rewrite-fn-begin
int match(int value) { return value; }
// @rewrite-fn-end
// @lowering-fn-end
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C" fn r#match({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let mut value: i32 = 0;
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: value = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn r#match({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = value;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
