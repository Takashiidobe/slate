_Thread_local int shared_value = 5;

// @rewrite-fn-begin
int read_shared_value(void) { return shared_value; }
// @rewrite-fn-end

// LOWERING: #[thread_local]
// LOWERING-NEXT: pub static mut shared_value: i32 = 5;

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn read_shared_value() -> i32 {
// REWRITES-DAG:     unsafe { shared_value }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn read_shared_value() -> i32 {
// REWRITES-X86_64-GNU-DAG:     unsafe { shared_value }
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn read_shared_value() -> i32 {
// REWRITES-AARCH64-GNU-DAG:     unsafe { shared_value }
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
