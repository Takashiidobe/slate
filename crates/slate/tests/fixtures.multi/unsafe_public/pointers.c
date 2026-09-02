// @lowering-fn-begin
// @rewrite-fn-begin
int read_ptr(int *p) { return *p; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn read_ptr({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn read_ptr({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-DAG:     unsafe { *{{arg[0-9]+}} }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
