int root_helper(void);

// @lowering-fn-begin
// @rewrite-fn-begin
int call_root_helper(void) { return root_helper(); }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = root_helper();
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// REWRITES-DAG:     root_helper()
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = root_helper();
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-AARCH64-GNU-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = root_helper();
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// REWRITES-X86_64-GNU-DAG:     root_helper()
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn call_root_helper() -> i32 {
// REWRITES-AARCH64-GNU-DAG:     root_helper()
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
