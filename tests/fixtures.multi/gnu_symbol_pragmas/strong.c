// @rewrite-fn-begin
int pragma_weak_alias(int value) { return value + 100; }
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn pragma_weak_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 100;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn pragma_weak_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     {{arg[0-9]+}} + 100
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn pragma_weak_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     {{arg[0-9]+}} + 100
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
