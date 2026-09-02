// @rewrite-fn-begin
int bump(int value, ...) { return value + 1; }
// @rewrite-fn-end

// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn bump(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn bump({{arg[0-9]+}}: i32, mut __slate_va_args: ...) -> i32 {
// REWRITES-DAG:     return {{arg[0-9]+}} + 1;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
