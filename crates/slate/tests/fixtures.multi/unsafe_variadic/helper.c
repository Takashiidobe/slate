// @rewrite-fn-begin
int bump(int value, ...) { return value + 1; }
// @rewrite-fn-end

// LOWERING-LABEL: {{^}}pub unsafe extern "C-unwind" fn bump(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn bump({{arg[0-9]+}}: i32, mut __slate_va_args: ...) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub unsafe extern "C-unwind" fn bump({{arg[0-9]+}}: i32, mut __slate_va_args: ...) -> i32 {
// REWRITES-X86_64-GNU-DAG:     {{arg[0-9]+}} + 1
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub unsafe extern "C-unwind" fn bump({{arg[0-9]+}}: i32, mut __slate_va_args: ...) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     {{arg[0-9]+}} + 1
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
