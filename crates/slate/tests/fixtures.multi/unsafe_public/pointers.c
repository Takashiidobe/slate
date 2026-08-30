// @rewrite-fn-begin
int read_ptr(int *p) { return *p; }
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn read_ptr({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = unsafe { *{{arg[0-9]+}} };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn read_ptr(
// LOWERING-DAG: unsafe { *_v{{[0-9]+}} }
// LOWERING: {{^}}}
