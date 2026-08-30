// @rewrite-fn-begin
int pragma_weak_alias(int value) { return value + 100; }
// @rewrite-fn-end
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn pragma_weak_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 100;
// REWRITES-DAG: __retval = value + {{_v[0-9]+}};
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
