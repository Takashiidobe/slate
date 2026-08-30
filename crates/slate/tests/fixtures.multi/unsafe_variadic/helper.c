// @rewrite-fn-begin
int bump(int value, ...) { return value + 1; }
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn bump({{arg[0-9]+}}: i32, mut __slate_va_args: ...) -> i32 {
// REWRITES-DAG: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: __retval = value + {{_v[0-9]+}};
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn bump(
// LOWERING: {{^}}}
